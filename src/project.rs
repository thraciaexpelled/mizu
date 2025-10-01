pub mod project {
	use std::env;
	use std::fs;
	use std::path::PathBuf;

	// A wrapper for fs::create_dir()
	pub fn mkdir(dir: String) -> i8 {
		log::info!("About to create directory '{}'", dir);

		match fs::create_dir(dir) {
			Ok(_) => {
				log::info!("Succesfully created directory");
				0
			}
			Err(e) => {
				log::error!("Failed to create directory!");
				log::error!("Error:	{}", e);
				-1
			}
		}
	}

	// A wrapper for env::current_dir()
	pub fn getcwd() -> Result<PathBuf, ()> {
		log::info!("About to get current working directory");

		match env::current_dir() {
			Ok(path) => {
				log::info!("Succesfully got path: {:?}", path);
				Ok(path)
			}
			Err(e) => {
				log::error!("Failed to get path: {}", e);
				Err(())
			}
		}
 	}

	pub fn get_dir_create_filepath(dir: String) -> String {
		let cwd = getcwd();
		let filepath =  cwd.expect("Cannot join path").join(dir);
		filepath.to_string_lossy().to_string()
	}
}