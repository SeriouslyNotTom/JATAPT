#include <Alcubierre/IO/FIleUtils.h>
#include <Alcubierre/Debug/Log.h>
#include <sys/stat.h>
#include <dirent.h>
#include <mio/mmap.hpp>


#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#include <cderr.h>
#include <iostream>

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

std::vector<char*> Alcubierre::File::Util::ListDir(const char* Path, AlcubierreListDirFlags dir_flags)
{

	bool o_Recurse = dir_flags & AlcubierreListDirFlags_Recurse;
	bool o_AppendSubdir = dir_flags & AlcubierreListDirFLags_AppendSubdirectoryPrefix;
	bool o_ReturnFolders = dir_flags & AlcubierreListDirFlags_ReturnFolders;
	bool o_NoFiles = dir_flags & AlcubierreListDirFlags_NoFiles;
	bool o_FullPath = dir_flags & AlcubierreListDirFlags_FullPath;

	std::vector<char*> results;
	std::vector<char*> subdirs;

	struct dirent *file_h;
	DIR *dir_h = opendir(Path);
	while ((file_h = readdir(dir_h)) != NULL)
	{
		switch (file_h->d_type)
		{
			case DT_DIR:
			{
				//don't use costly string compares if we are not even using any of the directory options in the first place
				if (o_Recurse | o_ReturnFolders)
				{
					if (strcmp(file_h->d_name, ".") != true & file_h->d_name != "..")
					{
						if (o_Recurse) { subdirs.push_back(file_h->d_name); }
						if (o_ReturnFolders) { results.push_back((file_h->d_name) + '/'); }
					}
				}
				break;
			}
			case DT_REG:
			{
				if (!o_NoFiles)
				{
					char* file_name = new char[strlen(file_h->d_name)];
					strcpy(file_name, file_h->d_name);
					results.push_back(file_name);
				}
				break;
			}

		}
	}

	//todo for release
	/*if (o_Recurse & subdirs.size()>0)
	{


		while (subdirs.size() > 0)
		{
			for (int i = 0; i <= subdirs.size(); i++)
			{

				char* sub = "";
				strcpy(sub, Path);
				strcpy(sub+strlen(sub), "/");
				strcpy(sub+strlen(sub), subdirs[i]);

				std::vector<char*> sub_dirs = ListDir(subdirs[i], AlcubierreListDirFlags_ReturnFolders);
				std::vector<char*> sub_results = ListDir(subdirs[i], AlcubierreListDirFlags_None);

				for (int k; i <= sub_results.size(); k++)
				{
					char* result = "";
					if (o_AppendSubdir)
					{
						strcpy(result, subdirs[i]);
						strcpy(result+strlen(result), "/");
						strcpy(result + strlen(result), sub_results[k]);
					}
					else {
						result = sub_results[k];
					}
				}
			}
		}
	}*/

	if (o_FullPath)
	{
		for (int i = 0; i <results.size(); i++)
		{
			char* strResult = new char[strlen(Path)];
			strcpy(strResult, Path);
			//size of 1 because of spooky escape codes
			char* strCheck = new char[1];
			strcpy(strCheck, strResult + (strlen(strResult) - 1));
			if (strcmp(strCheck, "\\"))
			{
				char* strNewResult = new char[strlen(strResult)+2];
				strcpy(strNewResult, strResult);
				strcat(strNewResult, "\\");
				strResult = strNewResult;
				strNewResult = nullptr;
			}
			char* strNewResult = new char[strlen(strResult) + strlen(results[i])];
			strcpy(strNewResult, strResult);
			strcat(strNewResult, results[i]);
			strResult = strNewResult;
			results[i] = strResult;
		}
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

