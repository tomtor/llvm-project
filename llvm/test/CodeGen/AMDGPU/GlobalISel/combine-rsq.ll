; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -global-isel -mtriple=amdgcn -mcpu=gfx1010 %s -o - | FileCheck -check-prefix=GCN %s

define amdgpu_cs float @div_sqrt(float inreg %arg1) {
; GCN-LABEL: div_sqrt:
; GCN:       ; %bb.0: ; %.entry
; GCN-NEXT:    v_mul_f32_e64 v0, 0x4f800000, s0
; GCN-NEXT:    v_cmp_gt_f32_e64 vcc_lo, 0xf800000, s0
; GCN-NEXT:    v_cndmask_b32_e32 v0, s0, v0, vcc_lo
; GCN-NEXT:    v_sqrt_f32_e32 v1, v0
; GCN-NEXT:    v_add_nc_u32_e32 v2, -1, v1
; GCN-NEXT:    v_add_nc_u32_e32 v3, 1, v1
; GCN-NEXT:    v_fma_f32 v4, -v2, v1, v0
; GCN-NEXT:    v_fma_f32 v5, -v3, v1, v0
; GCN-NEXT:    v_cmp_ge_f32_e64 s0, 0, v4
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v2, s0
; GCN-NEXT:    v_cmp_lt_f32_e64 s0, 0, v5
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v3, s0
; GCN-NEXT:    v_mul_f32_e32 v2, 0x37800000, v1
; GCN-NEXT:    v_cndmask_b32_e32 v1, v1, v2, vcc_lo
; GCN-NEXT:    v_cmp_class_f32_e64 vcc_lo, v0, 0x260
; GCN-NEXT:    v_cndmask_b32_e32 v0, v1, v0, vcc_lo
; GCN-NEXT:    v_rcp_f32_e32 v0, v0
; GCN-NEXT:    ; return to shader part epilog
.entry:
  %a = call float @llvm.sqrt.f32(float %arg1)
  %b = fdiv afn float 1.000000e+00, %a
  ret float %b
}

define amdgpu_cs float @sqrt_div(float inreg %arg1) {
; GCN-LABEL: sqrt_div:
; GCN:       ; %bb.0: ; %.entry
; GCN-NEXT:    v_rcp_f32_e32 v0, s0
; GCN-NEXT:    v_mul_f32_e32 v1, 0x4f800000, v0
; GCN-NEXT:    v_cmp_gt_f32_e32 vcc_lo, 0xf800000, v0
; GCN-NEXT:    v_cndmask_b32_e32 v0, v0, v1, vcc_lo
; GCN-NEXT:    v_sqrt_f32_e32 v1, v0
; GCN-NEXT:    v_add_nc_u32_e32 v2, -1, v1
; GCN-NEXT:    v_add_nc_u32_e32 v3, 1, v1
; GCN-NEXT:    v_fma_f32 v4, -v2, v1, v0
; GCN-NEXT:    v_fma_f32 v5, -v3, v1, v0
; GCN-NEXT:    v_cmp_ge_f32_e64 s0, 0, v4
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v2, s0
; GCN-NEXT:    v_cmp_lt_f32_e64 s0, 0, v5
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v3, s0
; GCN-NEXT:    v_mul_f32_e32 v2, 0x37800000, v1
; GCN-NEXT:    v_cndmask_b32_e32 v1, v1, v2, vcc_lo
; GCN-NEXT:    v_cmp_class_f32_e64 vcc_lo, v0, 0x260
; GCN-NEXT:    v_cndmask_b32_e32 v0, v1, v0, vcc_lo
; GCN-NEXT:    ; return to shader part epilog
.entry:
  %a = fdiv afn float 1.000000e+00, %arg1
  %b = call float @llvm.sqrt.f32(float %a)
  ret float %b
}

define amdgpu_cs float @rcp_sqrt(float inreg %arg1) {
; GCN-LABEL: rcp_sqrt:
; GCN:       ; %bb.0: ; %.entry
; GCN-NEXT:    v_mul_f32_e64 v0, 0x4f800000, s0
; GCN-NEXT:    v_cmp_gt_f32_e64 vcc_lo, 0xf800000, s0
; GCN-NEXT:    v_cndmask_b32_e32 v0, s0, v0, vcc_lo
; GCN-NEXT:    v_sqrt_f32_e32 v1, v0
; GCN-NEXT:    v_add_nc_u32_e32 v2, -1, v1
; GCN-NEXT:    v_add_nc_u32_e32 v3, 1, v1
; GCN-NEXT:    v_fma_f32 v4, -v2, v1, v0
; GCN-NEXT:    v_fma_f32 v5, -v3, v1, v0
; GCN-NEXT:    v_cmp_ge_f32_e64 s0, 0, v4
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v2, s0
; GCN-NEXT:    v_cmp_lt_f32_e64 s0, 0, v5
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v3, s0
; GCN-NEXT:    v_mul_f32_e32 v2, 0x37800000, v1
; GCN-NEXT:    v_cndmask_b32_e32 v1, v1, v2, vcc_lo
; GCN-NEXT:    v_cmp_class_f32_e64 vcc_lo, v0, 0x260
; GCN-NEXT:    v_cndmask_b32_e32 v0, v1, v0, vcc_lo
; GCN-NEXT:    v_rcp_f32_e32 v0, v0
; GCN-NEXT:    ; return to shader part epilog
.entry:
  %a = call float @llvm.sqrt.f32(float %arg1)
  %b = call float @llvm.amdgcn.rcp.f32(float %a)
  ret float %b
}

define amdgpu_cs float @sqrt_rcp(float inreg %arg1) {
; GCN-LABEL: sqrt_rcp:
; GCN:       ; %bb.0: ; %.entry
; GCN-NEXT:    v_rcp_f32_e32 v0, s0
; GCN-NEXT:    v_mul_f32_e32 v1, 0x4f800000, v0
; GCN-NEXT:    v_cmp_gt_f32_e32 vcc_lo, 0xf800000, v0
; GCN-NEXT:    v_cndmask_b32_e32 v0, v0, v1, vcc_lo
; GCN-NEXT:    v_sqrt_f32_e32 v1, v0
; GCN-NEXT:    v_add_nc_u32_e32 v2, -1, v1
; GCN-NEXT:    v_add_nc_u32_e32 v3, 1, v1
; GCN-NEXT:    v_fma_f32 v4, -v2, v1, v0
; GCN-NEXT:    v_fma_f32 v5, -v3, v1, v0
; GCN-NEXT:    v_cmp_ge_f32_e64 s0, 0, v4
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v2, s0
; GCN-NEXT:    v_cmp_lt_f32_e64 s0, 0, v5
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v3, s0
; GCN-NEXT:    v_mul_f32_e32 v2, 0x37800000, v1
; GCN-NEXT:    v_cndmask_b32_e32 v1, v1, v2, vcc_lo
; GCN-NEXT:    v_cmp_class_f32_e64 vcc_lo, v0, 0x260
; GCN-NEXT:    v_cndmask_b32_e32 v0, v1, v0, vcc_lo
; GCN-NEXT:    ; return to shader part epilog
.entry:
  %a = call float @llvm.amdgcn.rcp.f32(float %arg1)
  %b = call float @llvm.sqrt.f32(float %a)
  ret float %b
}

define amdgpu_cs float @div_sqrt_contract(float inreg %arg1) {
; GCN-LABEL: div_sqrt_contract:
; GCN:       ; %bb.0: ; %.entry
; GCN-NEXT:    v_mul_f32_e64 v0, 0x4f800000, s0
; GCN-NEXT:    v_cmp_gt_f32_e64 vcc_lo, 0xf800000, s0
; GCN-NEXT:    v_cndmask_b32_e32 v0, s0, v0, vcc_lo
; GCN-NEXT:    v_sqrt_f32_e32 v1, v0
; GCN-NEXT:    v_add_nc_u32_e32 v2, -1, v1
; GCN-NEXT:    v_add_nc_u32_e32 v3, 1, v1
; GCN-NEXT:    v_fma_f32 v4, -v2, v1, v0
; GCN-NEXT:    v_fma_f32 v5, -v3, v1, v0
; GCN-NEXT:    v_cmp_ge_f32_e64 s0, 0, v4
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v2, s0
; GCN-NEXT:    v_cmp_lt_f32_e64 s0, 0, v5
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v3, s0
; GCN-NEXT:    v_mul_f32_e32 v2, 0x37800000, v1
; GCN-NEXT:    v_cndmask_b32_e32 v1, v1, v2, vcc_lo
; GCN-NEXT:    v_cmp_class_f32_e64 vcc_lo, v0, 0x260
; GCN-NEXT:    v_cndmask_b32_e32 v0, v1, v0, vcc_lo
; GCN-NEXT:    v_rcp_f32_e32 v0, v0
; GCN-NEXT:    ; return to shader part epilog
.entry:
  %a = call contract float @llvm.sqrt.f32(float %arg1)
  %b = fdiv afn contract float 1.000000e+00, %a
  ret float %b
}

define amdgpu_cs float @sqrt_div_contract(float inreg %arg1) {
; GCN-LABEL: sqrt_div_contract:
; GCN:       ; %bb.0: ; %.entry
; GCN-NEXT:    v_rcp_f32_e32 v0, s0
; GCN-NEXT:    v_mul_f32_e32 v1, 0x4f800000, v0
; GCN-NEXT:    v_cmp_gt_f32_e32 vcc_lo, 0xf800000, v0
; GCN-NEXT:    v_cndmask_b32_e32 v0, v0, v1, vcc_lo
; GCN-NEXT:    v_sqrt_f32_e32 v1, v0
; GCN-NEXT:    v_add_nc_u32_e32 v2, -1, v1
; GCN-NEXT:    v_add_nc_u32_e32 v3, 1, v1
; GCN-NEXT:    v_fma_f32 v4, -v2, v1, v0
; GCN-NEXT:    v_fma_f32 v5, -v3, v1, v0
; GCN-NEXT:    v_cmp_ge_f32_e64 s0, 0, v4
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v2, s0
; GCN-NEXT:    v_cmp_lt_f32_e64 s0, 0, v5
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v3, s0
; GCN-NEXT:    v_mul_f32_e32 v2, 0x37800000, v1
; GCN-NEXT:    v_cndmask_b32_e32 v1, v1, v2, vcc_lo
; GCN-NEXT:    v_cmp_class_f32_e64 vcc_lo, v0, 0x260
; GCN-NEXT:    v_cndmask_b32_e32 v0, v1, v0, vcc_lo
; GCN-NEXT:    ; return to shader part epilog
.entry:
  %a = fdiv afn contract float 1.000000e+00, %arg1
  %b = call contract float @llvm.sqrt.f32(float %a)
  ret float %b
}

define amdgpu_cs float @rcp_sqrt_contract(float inreg %arg1) {
; GCN-LABEL: rcp_sqrt_contract:
; GCN:       ; %bb.0: ; %.entry
; GCN-NEXT:    v_mul_f32_e64 v0, 0x4f800000, s0
; GCN-NEXT:    v_cmp_gt_f32_e64 vcc_lo, 0xf800000, s0
; GCN-NEXT:    v_cndmask_b32_e32 v0, s0, v0, vcc_lo
; GCN-NEXT:    v_sqrt_f32_e32 v1, v0
; GCN-NEXT:    v_add_nc_u32_e32 v2, -1, v1
; GCN-NEXT:    v_add_nc_u32_e32 v3, 1, v1
; GCN-NEXT:    v_fma_f32 v4, -v2, v1, v0
; GCN-NEXT:    v_fma_f32 v5, -v3, v1, v0
; GCN-NEXT:    v_cmp_ge_f32_e64 s0, 0, v4
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v2, s0
; GCN-NEXT:    v_cmp_lt_f32_e64 s0, 0, v5
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v3, s0
; GCN-NEXT:    v_mul_f32_e32 v2, 0x37800000, v1
; GCN-NEXT:    v_cndmask_b32_e32 v1, v1, v2, vcc_lo
; GCN-NEXT:    v_cmp_class_f32_e64 vcc_lo, v0, 0x260
; GCN-NEXT:    v_cndmask_b32_e32 v0, v1, v0, vcc_lo
; GCN-NEXT:    v_rcp_f32_e32 v0, v0
; GCN-NEXT:    ; return to shader part epilog
.entry:
  %a = call contract float @llvm.sqrt.f32(float %arg1)
  %b = call contract float @llvm.amdgcn.rcp.f32(float %a)
  ret float %b
}

define amdgpu_cs float @sqrt_rcp_contract(float inreg %arg1) {
; GCN-LABEL: sqrt_rcp_contract:
; GCN:       ; %bb.0: ; %.entry
; GCN-NEXT:    v_rcp_f32_e32 v0, s0
; GCN-NEXT:    v_mul_f32_e32 v1, 0x4f800000, v0
; GCN-NEXT:    v_cmp_gt_f32_e32 vcc_lo, 0xf800000, v0
; GCN-NEXT:    v_cndmask_b32_e32 v0, v0, v1, vcc_lo
; GCN-NEXT:    v_sqrt_f32_e32 v1, v0
; GCN-NEXT:    v_add_nc_u32_e32 v2, -1, v1
; GCN-NEXT:    v_add_nc_u32_e32 v3, 1, v1
; GCN-NEXT:    v_fma_f32 v4, -v2, v1, v0
; GCN-NEXT:    v_fma_f32 v5, -v3, v1, v0
; GCN-NEXT:    v_cmp_ge_f32_e64 s0, 0, v4
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v2, s0
; GCN-NEXT:    v_cmp_lt_f32_e64 s0, 0, v5
; GCN-NEXT:    v_cndmask_b32_e64 v1, v1, v3, s0
; GCN-NEXT:    v_mul_f32_e32 v2, 0x37800000, v1
; GCN-NEXT:    v_cndmask_b32_e32 v1, v1, v2, vcc_lo
; GCN-NEXT:    v_cmp_class_f32_e64 vcc_lo, v0, 0x260
; GCN-NEXT:    v_cndmask_b32_e32 v0, v1, v0, vcc_lo
; GCN-NEXT:    ; return to shader part epilog
.entry:
  %a = call contract float @llvm.amdgcn.rcp.f32(float %arg1)
  %b = call contract float @llvm.sqrt.f32(float %a)
  ret float %b
}

declare float @llvm.sqrt.f32(float)
declare float @llvm.amdgcn.rcp.f32(float)
