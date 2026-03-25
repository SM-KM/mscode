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
