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
