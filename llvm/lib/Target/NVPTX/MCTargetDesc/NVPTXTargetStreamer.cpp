//=====- NVPTXTargetStreamer.cpp - NVPTXTargetStreamer class ------------=====//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the NVPTXTargetStreamer class.
//
//===----------------------------------------------------------------------===//

#include "NVPTXTargetStreamer.h"
#include "NVPTXUtilities.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Casting.h"

using namespace llvm;

//
// NVPTXTargetStreamer Implemenation
//
NVPTXTargetStreamer::NVPTXTargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}
NVPTXTargetStreamer::~NVPTXTargetStreamer() = default;

NVPTXAsmTargetStreamer::NVPTXAsmTargetStreamer(MCStreamer &S)
    : NVPTXTargetStreamer(S) {}
NVPTXAsmTargetStreamer::~NVPTXAsmTargetStreamer() = default;

void NVPTXTargetStreamer::outputDwarfFileDirectives() {
  for (const std::string &S : DwarfFiles)
    getStreamer().emitRawText(S);
  DwarfFiles.clear();
}

void NVPTXTargetStreamer::closeLastSection() {
  if (HasSections)
    getStreamer().emitRawText("\t}");
}

void NVPTXTargetStreamer::emitDwarfFileDirective(StringRef Directive) {
  DwarfFiles.emplace_back(Directive);
}

static bool isDwarfSection(const MCObjectFileInfo *FI,
                           const MCSection *Section) {
  // FIXME: the checks for the DWARF sections are very fragile and should be
  // fixed up in a followup patch.
  if (!Section || Section->isText())
    return false;
  return Section == FI->getDwarfAbbrevSection() ||
         Section == FI->getDwarfInfoSection() ||
         Section == FI->getDwarfMacinfoSection() ||
         Section == FI->getDwarfFrameSection() ||
         Section == FI->getDwarfAddrSection() ||
         Section == FI->getDwarfRangesSection() ||
         Section == FI->getDwarfARangesSection() ||
         Section == FI->getDwarfLocSection() ||
         Section == FI->getDwarfStrSection() ||
         Section == FI->getDwarfLineSection() ||
         Section == FI->getDwarfStrOffSection() ||
         Section == FI->getDwarfLineStrSection() ||
         Section == FI->getDwarfPubNamesSection() ||
         Section == FI->getDwarfPubTypesSection() ||
         Section == FI->getDwarfSwiftASTSection() ||
         Section == FI->getDwarfTypesDWOSection() ||
         Section == FI->getDwarfAbbrevDWOSection() ||
         Section == FI->getDwarfAccelObjCSection() ||
         Section == FI->getDwarfAccelNamesSection() ||
         Section == FI->getDwarfAccelTypesSection() ||
         Section == FI->getDwarfAccelNamespaceSection() ||
         Section == FI->getDwarfLocDWOSection() ||
         Section == FI->getDwarfStrDWOSection() ||
         Section == FI->getDwarfCUIndexSection() ||
         Section == FI->getDwarfInfoDWOSection() ||
         Section == FI->getDwarfLineDWOSection() ||
         Section == FI->getDwarfTUIndexSection() ||
         Section == FI->getDwarfStrOffDWOSection() ||
         Section == FI->getDwarfDebugNamesSection() ||
         Section == FI->getDwarfDebugInlineSection() ||
         Section == FI->getDwarfGnuPubNamesSection() ||
         Section == FI->getDwarfGnuPubTypesSection();
}

void NVPTXTargetStreamer::changeSection(const MCSection *CurSection,
                                        MCSection *Section, uint32_t SubSection,
                                        raw_ostream &OS) {
  assert(!SubSection && "SubSection is not null!");
  const MCObjectFileInfo *FI = getStreamer().getContext().getObjectFileInfo();
  // Emit closing brace for DWARF sections only.
  if (isDwarfSection(FI, CurSection))
    OS << "\t}\n";
  if (isDwarfSection(FI, Section)) {
    // Emit DWARF .file directives in the outermost scope.
    outputDwarfFileDirectives();
    OS << "\t.section\t" << Section->getName() << '\n';
    // DWARF sections are enclosed into braces - emit the open one.
    OS << "\t{\n";
    HasSections = true;
  }
}

void NVPTXTargetStreamer::emitRawBytes(StringRef Data) {
  MCTargetStreamer::emitRawBytes(Data);
  // TODO: enable this once the bug in the ptxas with the packed bytes is
  // resolved. Currently, (it is confirmed by NVidia) it causes a crash in
  // ptxas.
#if 0
  const MCAsmInfo *MAI = Streamer.getContext().getAsmInfo();
  const char *Directive = MAI->getData8bitsDirective();
  unsigned NumElements = Data.size();
  const unsigned MaxLen = 40;
  unsigned NumChunks = 1 + ((NumElements - 1) / MaxLen);
  // Split the very long directives into several parts if the limit is
  // specified.
  for (unsigned I = 0; I < NumChunks; ++I) {
    SmallString<128> Str;
    raw_svector_ostream OS(Str);

    const char *Label = Directive;
    for (auto It = std::next(Data.bytes_begin(), I * MaxLen),
              End = (I == NumChunks - 1)
                        ? Data.bytes_end()
                        : std::next(Data.bytes_begin(), (I + 1) * MaxLen);
         It != End; ++It) {
      OS << Label << (unsigned)*It;
      if (Label == Directive)
        Label = ",";
    }
    Streamer.emitRawText(OS.str());
  }
#endif
}

void NVPTXTargetStreamer::emitValue(const MCExpr *Value) {
  if (Value->getKind() == MCExpr::SymbolRef) {
    const MCSymbolRefExpr &SRE = cast<MCSymbolRefExpr>(*Value);
    StringRef SymName = SRE.getSymbol().getName();
    if (!SymName.starts_with(".debug")) {
      Streamer.emitRawText(NVPTX::getValidPTXIdentifier(SymName));
      return;
    }
    // Fall through to the normal printing.
  }
  // Otherwise, print the Value normally.
  MCTargetStreamer::emitValue(Value);
}
