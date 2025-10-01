// args.rs - manage arguments that were passed
// * this file is a part of `mizu`
// * `mizu` is MIT licensed by the authors of this software


use log;
use serde::Serialize;


use std::fs;
use std::io::ErrorKind;

use std::path::Path;

use crate::terminal::cli_basic;
use crate::project::project;

// Config: would not be needed elsewhere
// only wraps around SubcommandNew
#[derive(Serialize)]
struct Config {
	mizu_config: SubcommandNew,
}

// New: manage functionality for the subcommand `new`
#[derive(Serialize)]
pub struct SubcommandNew {
	name: String,
	proj_type: String,
	license: String,
}

pub enum FileStat {
	Exists,
	Directory,
}

impl SubcommandNew {
	/* UPDATE (1-10): private methods moved to project.rs */

	pub fn initialize_implementation(name: String, proj_type: String, license: String) -> SubcommandNew {
		SubcommandNew {
			name,
			proj_type,
			license
		}
	}

	pub fn ensure_sanity(&self) -> Result<FileStat, std::io::Error> {
		let binding = project::get_dir_create_filepath(self.name.clone());
  		let project_path = Path::new(&binding);
		log::info!("Checking if {:?} exists...", project_path);

		match fs::metadata(project_path) {
			Ok(meta) => {
				if meta.is_dir() {
					log::info!("{:?} exists and is a directory.", project_path);
					// TODO: check if that is our project or not
					Ok(FileStat::Directory)
				} else {
					log::warn!("{:?} exists but not as a directory. Need to create '{:?}'...", project_path, project_path);
					Ok(FileStat::Exists)
				}
			}
			Err(e) => {
				// don't log anything unless if it's not a NotFound error
				match e.kind() {
					ErrorKind::NotFound => Err(e),
					_ => {
						log::error!("Failed to check {:?}", project_path);
						log::error!("Error: {:?}", e);
						Err(e)
					}
				}
			}
		}
	}

	pub fn create_config_file(&self) -> Result<String, std::io::Error> {
		let config_opts: Config = Config {
			mizu_config: SubcommandNew {
				name: self.name.clone(),
				proj_type: self.proj_type.clone(),
				license: self.license.clone(),
			},
		};

		log::info!("Writing configuration file for project '{}'", self.name);
		let config = toml::to_string(&config_opts).unwrap();

		// join both project directory and config file
		let binding = project::get_dir_create_filepath(self.name.clone());
  		let project_path = Path::new(&binding);
  		let conf_file_name = "mizurc.toml";
  		let conf_file_path = project_path.join(conf_file_name);

		// now write config
		match fs::write(&conf_file_path, config) {
			Ok(_) => {
				log::info!("Succesfully wrote configuration file");
				Ok(conf_file_path.to_string_lossy().to_string())
			}
			Err(e) => {
				log::error!("Failed to write configuration file");
				Err(e)
			}
		}
	}

	pub fn start(&self) {
		log::info!("Creating project '{}'", self.name);

		match self.ensure_sanity() {
			Ok(FileStat::Directory) => {
				log::warn!("Project '{}' already exists in the current working directory", self.name);
				return;
			},
			Ok(FileStat::Exists) => {
				cli_basic::alert::creation(self.name.clone(), true);
				assert_eq!(project::mkdir(self.name.clone()), 0, "should not be erroneous");
			}
			Err(e) => {
				match e.kind() {
					ErrorKind::NotFound => {
						cli_basic::alert::creation(self.name.clone(), true);
						assert_eq!(project::mkdir(self.name.clone()), 0, "should not be erroneous")
					}
					_ => log::trace!("{}", e),
				}
			},
		}

		match self.create_config_file() {
			Ok(conf_path) => {
				cli_basic::alert::creation(conf_path, false);
			}
			Err(e) => log::error!("{}", e),
		}

		cli_basic::normal::project_created(self.proj_type.clone(), self.name.clone());
	}
}
