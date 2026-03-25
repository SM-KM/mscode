fn apply<F>(f: F)
where
    F: FnOnce(),
{
    f();
}

fn apply_to_3<F>(f: F) -> i32
where
    F: Fn(i32) -> i32,
{
    f(3)
}

fn function() {}
fn call_me<F: Fn()>(f: F) {
    f();
}

// returnning closures as output parameters
fn c_fn() -> impl Fn() {
    let text = "Fn".to_owned();
    move || println!("returning a closure, {}", text)
}

pub fn closures() {
    let outer = 5;
    let closure_annotaded = |i: i32| -> i32 { i + outer };
    let print = || println!("somthing");
    let closure_not = |i: i32| i + outer;

    println!("closure_annotated: {}", closure_annotaded(1));
    println!("closure_inferred: {}", closure_not(1));

    let one = || 1;
    println!("return 1: {}", one());

    let hay = vec![1, 2, 3];
    let contains = move |needle| hay.contains(needle);
    contains(&2);

    call_me(print);
    call_me(function);
}
