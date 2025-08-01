// RUN: %clang_cc1 -triple x86_64-linux-android -emit-llvm -O -o - %s \
// RUN:    | FileCheck %s --check-prefix=ANDROID --check-prefix=CHECK
// RUN: %clang_cc1 -triple x86_64-linux-gnu -emit-llvm -O -o - %s \
// RUN:    | FileCheck %s --check-prefix=GNU --check-prefix=CHECK
// RUN: %clang_cc1 -triple x86_64 -emit-llvm -O -o - %s \
// RUN:    | FileCheck %s --check-prefix=GNU --check-prefix=CHECK

// Android uses fp128 for long double but other x86_64 targets use x86_fp80.

long double dataLD = 1.0L;
// ANDROID: @dataLD ={{.*}} local_unnamed_addr global fp128 0xL00000000000000003FFF000000000000, align 16
// GNU: @dataLD ={{.*}} local_unnamed_addr global x86_fp80 0xK3FFF8000000000000000, align 16

long double _Complex dataLDC = {1.0L, 1.0L};
// ANDROID: @dataLDC ={{.*}} local_unnamed_addr global { fp128, fp128 } { fp128 0xL00000000000000003FFF000000000000, fp128 0xL00000000000000003FFF000000000000 }, align 16
// GNU: @dataLDC ={{.*}} local_unnamed_addr global { x86_fp80, x86_fp80 } { x86_fp80 0xK3FFF8000000000000000, x86_fp80 0xK3FFF8000000000000000 }, align 16

long double TestLD(long double x) {
  return x * x;
// ANDROID: define{{.*}} fp128 @TestLD(fp128 noundef %x)
// GNU: define{{.*}} x86_fp80 @TestLD(x86_fp80 noundef %x)
}

long double _Complex TestLDC(long double _Complex x) {
  return x * x;
// ANDROID: define{{.*}} void @TestLDC(ptr {{.*}}, ptr {{.*}} %x)
// GNU: define{{.*}} { x86_fp80, x86_fp80 } @TestLDC(ptr {{.*}} %x)
}

typedef __builtin_va_list va_list;

int TestGetVarInt(va_list ap) {
  return __builtin_va_arg(ap, int);
// Since int can be passed in memory or register there are two branches.
// CHECK:   define{{.*}} i32 @TestGetVarInt(
// CHECK:   br label
// CHECK:   br label
// CHECK:   = phi
// CHECK:   ret i32
}

double TestGetVarDouble(va_list ap) {
  return __builtin_va_arg(ap, double);
// Since double can be passed in memory or register there are two branches.
// CHECK:   define{{.*}} double @TestGetVarDouble(
// CHECK:   br label
// CHECK:   br label
// CHECK:   = phi
// CHECK:   ret double
}

long double TestGetVarLD(va_list ap) {
  return __builtin_va_arg(ap, long double);
// fp128 and double can be passed in memory or in register, but x86_fp80 is in
// memory.
// ANDROID: define{{.*}} fp128 @TestGetVarLD(
// GNU:     define{{.*}} x86_fp80 @TestGetVarLD(
// ANDROID: br label
// ANDROID: br label
// ANDROID: = phi
// GNU-NOT: br
// GNU-NOT: = phi
// ANDROID: ret fp128
// GNU:     ret x86_fp80
}

long double _Complex TestGetVarLDC(va_list ap) {
  return __builtin_va_arg(ap, long double _Complex);
// Pair of fp128 or x86_fp80 are passed as struct in memory.
// ANDROID:   define{{.*}} void @TestGetVarLDC(ptr {{.*}}, ptr
// GNU:       define{{.*}} { x86_fp80, x86_fp80 } @TestGetVarLDC(
// Pair of double can go in SSE registers or memory
// ANDROID-NOT: br
// GNU-NOT: br
// ANDROID-NOT: phi
// GNU-NOT: phi
// ANDROID:   ret void
// GNU:       ret { x86_fp80, x86_fp80 }
}

void TestVarArg(const char *s, ...);

void TestPassVarInt(int x) {
  TestVarArg("A", x);
// CHECK: define{{.*}} void @TestPassVarInt(i32 noundef %x)
// CHECK: call {{.*}} @TestVarArg(ptr {{.*}}, i32 noundef %x)
}

void TestPassVarFloat(float x) {
  TestVarArg("A", x);
// CHECK: define{{.*}} void @TestPassVarFloat(float noundef %x)
// CHECK: call {{.*}} @TestVarArg(ptr {{.*}}, double noundef %
}

void TestPassVarDouble(double x) {
  TestVarArg("A", x);
// CHECK: define{{.*}} void @TestPassVarDouble(double noundef %x)
// CHECK: call {{.*}} @TestVarArg(ptr {{.*}}, double noundef %x
}

void TestPassVarLD(long double x) {
  TestVarArg("A", x);
// ANDROID: define{{.*}} void @TestPassVarLD(fp128 noundef %x)
// ANDROID: call {{.*}} @TestVarArg(ptr {{.*}}, fp128 noundef %x
// GNU: define{{.*}} void @TestPassVarLD(x86_fp80 noundef %x)
// GNU: call {{.*}} @TestVarArg(ptr {{.*}}, x86_fp80 noundef %x
}

void TestPassVarLDC(long double _Complex x) {
  TestVarArg("A", x);
// ANDROID:      define{{.*}} void @TestPassVarLDC(ptr {{.*}} %x)
// ANDROID:      store fp128 %{{.*}}, ptr %
// ANDROID-NEXT: store fp128 %{{.*}}, ptr %
// ANDROID-NEXT: call {{.*}} @TestVarArg(ptr {{.*}}, ptr {{.*}} %
// GNU:          define{{.*}} void @TestPassVarLDC(ptr {{.*}} %x)
// GNU:          store x86_fp80 %{{.*}}, ptr %
// GNU-NEXT:     store x86_fp80 %{{.*}}, ptr %
// GNU-NEXT:   call {{.*}} @TestVarArg(ptr {{.*}}, ptr {{.*}} %
}
