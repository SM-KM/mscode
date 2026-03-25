pub fn expressions() {
    // labels for loops, to break them from the
    // inner parts
    'outer: loop {
        'inner: loop {
            break 'outer;
        }
    }

    let x = 5u32;
    let y = {
        let x_squared = x * x;
        let x_cube = x_squared * x;

        // This expression will be assigned to `y`
        x_cube + x_squared + x
    };

    let z = {
        // The semicolon suppresses this expression and `()` is assigned to `z`
        2 * x;
    };
    println!("y: {:?}, z: {:?}", y, z);
}

pub fn binding() {
    match 15 {
        0 => println!("nothing"),
        n @ 1..=12 => println!("child of age {}", n),
        n @ 13..=19 => println!("teen fo age {}", n),
        n => println!("old person: {}", n),
    }
}
