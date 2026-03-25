#![allow(dead_code)]
#[derive(Debug)]
struct Person {
    name: String,
    age: u8,
}

struct Unit;
struct Pair(f32, f32);

#[derive(Clone, Copy, Debug)]
struct Point {
    x: f32,
    y: f32,
}

// structs can be reused, as fields in other structs
#[derive(Debug)]
struct Rectangle {
    top_left: Point,
    bottom_right: Point,
    width: f32,
    height: f32,
}

pub fn structures() {
    let another_point: Point = Point { x: 10.3, y: 0.2 };
    let bottom_right = Point { x: 10.3, ..another_point };
    println!("{:?}", bottom_right);
    println!("{:?}", square(another_point, 30.4));
}

fn square(point: Point, size: f32) -> Rectangle {
    Rectangle { top_left: point, bottom_right: point, width: size, height: size }
}

enum WebEvent {
    PageLoad,
    PageUnload,
    KeyPress(char),
    Paste(String),
    Click { x: i64, y: i64 },
}

fn inspect(evt: WebEvent) {
    match evt {
        WebEvent::PageLoad => println!("page loaded"),
        WebEvent::PageUnload => println!("page unloaded"),
        WebEvent::KeyPress(c) => println!("pressed '{}'.", c),
        WebEvent::Paste(s) => println!("pasted \"{}\".", s),
        WebEvent::Click { x, y } => {
            println!("clicked at x={}, y={}.", x, y);
        }
    }
}

// type aliases
enum VeryVerboseNameOfEnumToDoWithNumbers {
    Add,
    Substract,
}
type Operations = VeryVerboseNameOfEnumToDoWithNumbers;

enum Stage {
    Begginer,
    Advanced,
}

enum Role {
    Student,
    Teacher,
}

pub fn enumss() {
    let pressed = WebEvent::KeyPress('x');
    let pasted = WebEvent::Paste("my text".to_owned());
    let click = WebEvent::Click { x: 20, y: 80 };
    let load = WebEvent::PageLoad;
    let unload = WebEvent::PageUnload;

    inspect(pressed);
    inspect(pasted);
    inspect(click);
    inspect(load);
    inspect(unload);

    // avoid the scoping using use for
    // enums and modules
    use Role::*;
    use Stage::{Advanced, Begginer};
    let _stage = Begginer;
    let _role = Student;
}
