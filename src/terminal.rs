// terminal.rs - print messages that concerns the user
// * this file is a part of `mizu`
// * `mizu` is MIT licensed by the authors of this software


pub mod cli_basic {
	pub mod alert {
		pub fn creation(name: String, is_dir: bool) {
			if is_dir {
				println!("	{}:	{}", ansi_term::Colour::Green.paint("create-dir"), ansi_term::Colour::Yellow.paint(name));															
				return;
			}
			println!("	{}:	{}", ansi_term::Colour::Green.paint("create"), ansi_term::Colour::Yellow.paint(name));
		}


		// TODO: they are to be used soon...
		pub fn deletion(name: String, is_dir: bool) {
			if is_dir {
				println!("	{}:	{}", ansi_term::Colour::Red.paint("delete-dir"), ansi_term::Colour::Yellow.paint(name));
				return
			}
			println!("	{}:	{}", ansi_term::Colour::Red.paint("delete"), ansi_term::Colour::Yellow.paint(name));
		}
	}

	// TODO: they are to be used soon...
	pub mod normal {
		pub fn project_created(proj_type: String, name: String) {
			println!("\n 	created {} project {}", ansi_term::Colour::Purple.paint(proj_type), ansi_term::Colour::Yellow.paint(name));
		}

		pub fn project_initialized(proj_type: String, name: String) {
			println!("\n 	initialized {} project in {}", ansi_term::Colour::Purple.paint(proj_type), ansi_term::Colour::Yellow.paint(name));
		}
	}
}