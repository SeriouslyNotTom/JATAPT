#include <Alcubierre/IO/FIleUtils.h>
#include <Alcubierre/Debug/Log.h>
#include <sys/stat.h>
#include <dirent.h>
#include <mio/mmap.hpp>
#include <Alcubierre/Libraries/Utilities/RandomUtils.h>
#define MINIMP3_IMPLEMENTATION
#include <minimp3.h>
#include <iostream>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#include <cderr.h>


char* BrowseFile(char* Title, char* Filter)
{
	Alcubierre::Debug::Log::Msg("Info","Spawning File Dialogue");
	char filename[MAX_PATH];
	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = Filter;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = Title;
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		Alcubierre::Debug::Log::Msg("Info","Selected File: %s", filename);
		return filename;
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above.
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default:
			std::cout << "You cancelled.\n";
			return NULL;
			break;
		}
	}
}
#endif

#ifdef LINUX
char* BrowseFile(char* Title, char* Filter)
{
	return "";
}
#endif // LINUX

char* Alcubierre::File::Util::BrowseFileSingle(char* Title, char* filter)
{
	return BrowseFile(Title, filter);
}

std::vector<std::string> Alcubierre::File::Util::ListDir(const char* Path, AlcubierreListDirFlags dir_flags)
{

	bool o_Recurse = dir_flags & AlcubierreListDirFlags_Recurse;
	bool o_AppendSubdir = dir_flags & AlcubierreListDirFLags_AppendSubdirectoryPrefix;
	bool o_ReturnFolders = dir_flags & AlcubierreListDirFlags_ReturnFolders;
	bool o_NoFiles = dir_flags & AlcubierreListDirFlags_NoFiles;
	bool o_FullPath = dir_flags & AlcubierreListDirFlags_FullPath;

	std::vector<std::string> results;
	std::vector<std::string> subdirs;

	//need to know current dirrectory for adding the path to recursions
	std::string cur_path;
	std::string cur_subdir = "";
	cur_path = Path;

	//for directory recursion
	bool first_run = true;

	while (subdirs.size() > 0 | first_run)
	{
		first_run = false;

		if (o_Recurse & subdirs.size() > 0)
		{
			if (subdirs.size() > 1)
			{
				std::vector<std::string> new_subdirs;
				for (int i = 1; i <subdirs.size(); i++)
				{
					new_subdirs.push_back(subdirs[i]);
				}
				const char* new_subdir = subdirs[0].c_str();
				cur_subdir = new_subdir;
				cur_path = Alcubierre::File::Util::AddPath(Path, new_subdir, false);
				subdirs = new_subdirs;
				
			}
			else {
				const char* new_subdir = subdirs[0].c_str();
				cur_subdir = new_subdir;
				cur_path = Alcubierre::File::Util::AddPath(Path, new_subdir, false);
				subdirs = std::vector<std::string>();
			}
			
		}

		//lookup dirent if you are confused, everything gets iteratored via a single object type, and the d_type changes depending on the returned type (file, directory)
		struct dirent* file_h;
		DIR* dir_h = opendir(cur_path.c_str());
		while ((file_h = readdir(dir_h)) != NULL)
		{
			switch (file_h->d_type)
			{
			
			//dirent switches
			//directory
			case DT_DIR:
			{
				//don't use costly string compares if we are not even using any of the directory options in the first place
				if (o_Recurse | o_ReturnFolders)
				{
					//check if directories are "." or ".." because that's just current and previous
					if ((file_h->d_name[0]==46) != true & std::string(file_h->d_name)!=std::string(".."))
					{
						if (o_Recurse)
						{ 
							subdirs.push_back(AddPath(cur_subdir.c_str(),file_h->d_name)); 
						}
						if (o_ReturnFolders) { results.push_back((file_h->d_name) + '/'); }
					}
				}
				break;
			}
			//file
			case DT_REG:
			{
				if (!o_NoFiles)
				{

					if (o_AppendSubdir & cur_subdir!="")
					{
						results.push_back(AddPath(cur_subdir.c_str(),file_h->d_name,false));
					}
					else {
						results.push_back(std::string(file_h->d_name));
					}
					fprintf(stdout,"vec:result:%s\r\n",results[results.size()-1].c_str());
				}
				break;
			}

			}
		}

	}

	if (o_FullPath)
	{
		std::vector<std::string> new_results;
		for (int i = 0; i < results.size(); i++)
		{
			new_results.push_back(AddPath(Path, results[i].c_str(),false));
		}
		results = new_results;
	}

	return results;
}

bool Alcubierre::File::Util::FileExists(const char* Path, struct stat* stat_out)
{
	struct stat buff;
	bool exists = stat(Path, &buff)==0;
	if (stat_out != nullptr)
	{
		memcpy(stat_out, &buff, sizeof(buff));
	}
	return exists;
}

char* Alcubierre::File::Util::Load_File_memmap(const char* path)
{
	const mio::mmap_source mmap(path);
	char* data = new char[mmap.length()];
	memset(data, 0x00, sizeof(data));
	memcpy(data, mmap.data(), mmap.length());
	data[mmap.size()] = '\0';
	return data;
}

bool Alcubierre::File::Util::Save_File_memmap(const char* path, char* data)
{
	return false;
}

std::vector<char*> Alcubierre::File::Util::Strip_FileTypes(std::vector<char*> input_vector, const char* File_Type, bool invert)
{

	if (input_vector.size() <= 0) { return input_vector; }
	std::vector<char*> out_vector = std::vector<char*>();

	int check_size = strlen(File_Type);
	char* check_chunk = new char[check_size];

	for (int i = 0; i < input_vector.size(); i++)
	{
		char* vec_element = input_vector[i];
		strncpy(check_chunk, vec_element + (strlen(vec_element) - check_size), check_size);
		check_chunk[check_size] = '\0';
		if ((std::string(check_chunk) == std::string(File_Type))== invert)
		{
			out_vector.push_back(vec_element);
		}
	}

	return out_vector;
}

const char* Alcubierre::File::Util::ConformPath(const char* path, const char* path_divider)
{
	char* conformed_path = new char[strlen(path)*2];
	int conformed_path_final_len = 0;

	//doctor freeman ammo cache here
	auto add_seq = [&]()
	{
		for (int i = 0; i <strlen(path_divider); i++)
		{
			conformed_path[conformed_path_final_len] = path_divider[i];
			conformed_path_final_len++;
		}
	};

	for (int i = 0; i <strlen(path); i++)
	{
		char check_char = path[i];
		if (check_char == '/')
		{
			add_seq();
		}
		else {
			if (check_char == '\\')
			{
				if (i != strlen(path) - 1) {
					if (path[i + 1] == '\\')
					{
						i++;
						add_seq();
					}
				}
				else {
					add_seq();
				}
			}
			else {
				conformed_path[conformed_path_final_len] = path[i];
				conformed_path_final_len++;
			}
		}
		
	}

	char* final_path = new char[conformed_path_final_len];
	strncpy(final_path, conformed_path, conformed_path_final_len);
	final_path[conformed_path_final_len] = '\0';

	return final_path;
}

std::string Alcubierre::File::Util::AddPath(const char* path1, const char* path2, bool replace_divider, const char* path_divider)
{
	char* working_path1;
	char* working_path2;
	std::string final_path;
	
	if (replace_divider) { working_path1 = (char*)ConformPath(path1, path_divider); working_path2 = (char*)ConformPath(path2, path_divider); }
	else 
	{ 
		working_path1 = (char*)path1;
		working_path2 = (char*)path2;
	}

	char check_char1 = working_path1[strlen(working_path1)-1];
	char check_char2 = working_path2[0];

	if (check_char1==path_divider[0])
	{
		if (check_char2 == path_divider[0])
		{
			int path_divider_len = strlen(path_divider);
			int path2_len = strlen(working_path2);
			char* conformed_string = new char[path2_len - path_divider_len];
			memcpy(conformed_string, working_path2 + path_divider_len, (path2_len - path_divider_len));
			conformed_string[path2_len-path_divider_len] = '\0';
			final_path = std::string(working_path1) + std::string(conformed_string);
		}
		else {
			final_path = std::string(working_path1) + std::string(working_path2);
		}
	}
	else {
		if (check_char2 == path_divider[0])
		{
			final_path = std::string(working_path1) + std::string(working_path2);
		}
		else {
			final_path = std::string(working_path1) + std::string(path_divider) + std::string(working_path2);
		}
		
	}

	return final_path;
}

int Alcubierre::File::Util::Mp3FileDuration(const char* path1)
{

	struct stat buff;
	int result = stat(path1, &buff);
	if (result==-1) { throw Mp3FileDuration_Exception_FileDoesNotExist(); }

	uint8_t* file_buffer = new uint8_t[16384];
	mio::mmap_source mmap(path1, 0, 16384);
	memcpy(file_buffer, mmap.data(), 16384);
	mmap.unmap();

	static mp3dec_t mp3d;
	mp3dec_init(&mp3d);
	mp3dec_frame_info_t info;
	short pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];

	//decode to get info
	int samples = mp3dec_decode_frame(&mp3d, file_buffer, 16384, pcm, &info);
	
	//i don't know where this offset comes from but it works
	int offset = 71;

	//duration is audio size divided by bitrate
	int file_size_corrected = (int)buff.st_size + (int)info.frame_offset;
	int file_seconds = (file_size_corrected*8/1024) / (int)info.bitrate_kbps+(int)offset;

	int minutes = floor(file_seconds / 60);
	int seconds = file_seconds - (minutes * 60);

	return file_seconds;
}
