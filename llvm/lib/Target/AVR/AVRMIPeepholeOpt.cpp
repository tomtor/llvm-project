//===-- AVRPeepholeOpt.cpp - Peephole optimization ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains a pass for AVR specific peephole optimizations.
//
//===----------------------------------------------------------------------===//

#include "AVR.h"
#include "AVRInstrInfo.h"
#include "AVRTargetMachine.h"
#include "MCTargetDesc/AVRMCTargetDesc.h"

#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#include "AVRGenInstrInfo.inc"

using namespace llvm;

#define AVR_PEEPHOLE_NAME "AVR peephole optimization pass"

namespace {

/// Optimize machine code by removing unneeded instructions.
class AVRPeephole : public MachineFunctionPass {
public:
  static char ID;

  AVRPeephole() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

  StringRef getPassName() const override { return AVR_PEEPHOLE_NAME; }

private:
  typedef MachineBasicBlock Block;
  typedef Block::iterator BlockIt;

  bool inspectMBB(Block &MBB);
  template <unsigned OP> bool inspect(Block &MBB, BlockIt MBBI);
};

char AVRPeephole::ID = 0;

bool AVRPeephole::inspectMBB(MachineBasicBlock &MBB) {
  /*
     Example sequence (from ashr expansion)

     clr     r25
     mov     r25, r24
     clr     r24
     mov     r24, r25
  */

  BlockIt MBBI = MBB.begin(), E = MBB.end();
  while (MBBI != E) {
    BlockIt NMBBI = std::next(MBBI);
    if (NMBBI == E)
      break;
    auto opcode = MBBI->getOpcode();
    auto nextcode = NMBBI->getOpcode();

    // clr     r25
    // mov     r25, r24
    if (opcode == AVR::EORRdRr &&
        MBBI->getOperand(0).getReg() == MBBI->getOperand(1).getReg()) {
      if (nextcode == AVR::MOVRdRr &&
          NMBBI->getOperand(0).getReg() == MBBI->getOperand(0).getReg()) {
        MBBI->removeFromParent();
        return true;
      }
    }

    // mov     r25, r24
    // mov     r24, r25
    if (opcode == AVR::MOVRdRr) {
      if (nextcode == AVR::MOVRdRr &&
          NMBBI->getOperand(0).getReg() == MBBI->getOperand(1).getReg() &&
          NMBBI->getOperand(1).getReg() == MBBI->getOperand(0).getReg()) {
        NMBBI->removeFromParent();
        return true;
      }
    }
    MBBI = NMBBI;
  }

  return false;
}

bool AVRPeephole::runOnMachineFunction(MachineFunction &MF) {
  bool Modified = false;

  for (Block &MBB : MF) {
    bool ContinueInspecting = true;
    do {
      bool BlockModified = inspectMBB(MBB);
      Modified |= BlockModified;

      ContinueInspecting = BlockModified;
    } while (ContinueInspecting);
  }

  return Modified;
}

} // end of namespace llvm

INITIALIZE_PASS(AVRPeephole, "avr-peephole", AVR_PEEPHOLE_NAME,
                false, false)

FunctionPass *llvm::createAVRPeepholePass() {
  return new AVRPeephole();
}

