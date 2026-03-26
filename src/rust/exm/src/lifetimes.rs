#![allow(dead_code)]
fn print_refs<'a, 'b>(x: &'a i32, y: &'b i32) {
    println!("x {}, y {}", x, y);
}

// mutable references lifetimes
// returning lifetimes references is also allowed, as long as
// the lifetimes returning corresponds to the input
fn add_one<'a>(x: &'a mut i32) -> &'a mut i32 {
    x
}

// lifetimes with methods
struct Owner(i32);
impl Owner {
    fn add_one<'a>(&'a mut self) {
        self.0 += 1;
    }
    fn print<'a>(&'a self) {
        println!("val: {}", self.0);
    }
}

// lifetimes with bounds
#[derive(Debug)]
struct Ref<'a, T: 'a>(&'a T);

// lifetimes coercion -> lifetime 'a needs to be as long as lifetime 'a
fn choose_first<'a: 'b, 'b>(first: &'a i32, _: &'b i32) -> &'b i32 {
    first
}

pub fn lifetimes() {
    let (fo, nin) = (4, 9);
    print_refs(&fo, &nin);
}

// ellision
// the compiler add common lifetimes when not defined
// to improve readability
