; ModuleID = 'reentrantSample.c'
source_filename = "reentrantSample.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@globalVar = common dso_local global i32 0, align 4
@callTime = common dso_local global i32 0, align 4
@flawFunc1.flawInt = internal global i32 0, align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fineFunc() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 1, i32* %1, align 4
  store i32 2, i32* %2, align 4
  %3 = load i32, i32* %1, align 4
  %4 = load i32, i32* %2, align 4
  %5 = mul nsw i32 %4, %3
  store i32 %5, i32* %2, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @flawFunc() #0 {
  %1 = load i32, i32* @globalVar, align 4
  %2 = add nsw i32 %1, 1
  store i32 %2, i32* @globalVar, align 4
  %3 = load i32, i32* @callTime, align 4
  %4 = icmp eq i32 %3, 5
  br i1 %4, label %5, label %8

5:                                                ; preds = %0
  %6 = load i32, i32* @callTime, align 4
  %7 = add nsw i32 %6, 1
  store i32 %7, i32* @callTime, align 4
  call void @fineFunc()
  br label %11

8:                                                ; preds = %0
  %9 = load i32, i32* @callTime, align 4
  %10 = add nsw i32 %9, 1
  store i32 %10, i32* @callTime, align 4
  call void @flawFunc()
  br label %11

11:                                               ; preds = %8, %5
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @flawFunc1() #0 {
  %1 = alloca i32*, align 8
  %2 = call i8* @malloc(i64 4)
  %3 = bitcast i8* %2 to i32*
  store i32* %3, i32** %1, align 8
  %4 = load i32, i32* @flawFunc1.flawInt, align 4
  %5 = add nsw i32 %4, 1
  store i32 %5, i32* @flawFunc1.flawInt, align 4
  %6 = load i32*, i32** %1, align 8
  %7 = call i32 (i32*, ...) bitcast (i32 (...)* @free to i32 (i32*, ...)*)(i32* %6)
  ret void
}

declare dso_local i8* @malloc(i64) #1

declare dso_local i32 @free(...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void @fineFunc()
  call void @flawFunc()
  call void @flawFunc1()
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 10.0.0-4ubuntu1 "}
