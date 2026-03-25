use std::fmt;

// the normal types unsigned, signed ints, on top floats, and
// chars, bools, the important thing is the unit type ()

fn reverse(pair: (i32, bool)) -> (bool, i32) {
    // similar to structure bindings in c++, using let to
    // assign the values of tupples to variables.
    let (int_param, bool_param) = pair;
    (bool_param, int_param)
}

fn transpose(matrix: Matrix) -> Matrix {
    let a = matrix.0;
    let b = matrix.1;
    let c = matrix.2;
    let d = matrix.3;
    Matrix(a, c, b, d)
}

#[derive(Debug)]
struct Matrix(f32, f32, f32, f32);

impl fmt::Display for Matrix {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let _ = write!(f, "( {}, {} )\n", self.0, self.1);
        write!(f, "( {}, {} )", self.2, self.3)
    }
}

pub fn primitives() {
    let _arr: [i32; 5] = [1, 2, 3123, 413, 4];

    // more than 12 elemnts cannot be princted by deafult tho
    let tpl = ((1u8, 2u16, 2u32), (4u64, -1i8), -2i16);
    println!("{:?}", tpl);
    let matrix = Matrix(1.1, 1.2, 2.1, 2.2);

    println!("Matrix:\n{}", matrix);
    println!("Transpose:\n{}", transpose(matrix));
}

use std::mem;

fn analyze_slice(slice: &[i32]) {
    println!("first element of the slice: {}", slice[0]);
    println!("the slice has {} elements", slice.len());
}

pub fn arr_slices() {
    let xs: [i32; 5] = [1, 2, 3, 4, 5];
    println!("Number of elements in array: {}", xs.len());

    // are stack allocated
    println!("Array occupies {} bytes", mem::size_of_val(&xs));
}
