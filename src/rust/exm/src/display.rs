use std::fmt::{self};

struct City {
    name: &'static str,
    lat: f32,
    lon: f32,
}

struct List(Vec<i32>);

// Display a way to implement a different visualization
// for a type
impl fmt::Display for List {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let vec = &self.0;
        write!(f, "[")?;
        for (index, v) in vec.iter().enumerate() {
            if index != 0 {
                write!(f, ", ")?;
            }
            write!(f, "{}: {}", index, v)?;
        }
        write!(f, "]")
    }
}

#[derive(Debug)]
struct Color {
    red: u8,
    green: u8,
    blue: u8,
}

impl fmt::Display for City {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let lat_c = if self.lat >= 0.0 { 'N' } else { 'S' };
        let lon_c = if self.lon >= 0.0 { 'E' } else { 'W' };

        // is the same as format! but it writes
        // the result into a buffer
        write!(f, "{}: {:.3}{} {:.3}{}", self.name, self.lat.abs(), lat_c, self.lon.abs(), lon_c)
    }
}

pub fn display() {
    let v = List(vec![1, 2, 3]);
    println!("{}", v);
}
