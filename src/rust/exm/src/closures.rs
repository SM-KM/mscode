pub fn closures() {
    let outer = 5;
    let closure_annotaded = |i: i32| -> i32 { i + outer };
    let closure_not = |i: i32| i + outer;

    println!("closure_annotated: {}", closure_annotaded(1));
    println!("closure_inferred: {}", closure_not(1));

    let one = || 1;
    println!("return 1: {}", one());
}
