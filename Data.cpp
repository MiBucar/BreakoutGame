#include "Data.h"

void Data::AddScore(int score)
{
	mFileO[FILE_SCORES].open("Data//Scores.txt", std::ios::app);
	mFileO[FILE_SCORES] << score << "\n";
	mFileO[FILE_SCORES].close();
}

int Data::GetHighScore()
{
	int highScore = GetTopFiveScores().at(GetTopFiveScores().size() - 1);

	return highScore;
}

void Data::RestartScores()
{
	mFileO[FILE_SCORES].open("data//Scores.txt");

	mFileO[FILE_SCORES] << "0\n" << "0\n" << "0\n" << "0\n" << "0\n";

	mFileO[FILE_SCORES].close();
}

std::vector<int> Data::GetTopFiveScores()
{
	mFileI[FILE_SCORES].open("Data//Scores.txt");

	std::string line;
	std::vector<int> arr;

	while (std::getline(mFileI[FILE_SCORES], line)) {
		int num;
		std::stringstream ss;
		ss << line;
		ss >> num;
		arr.push_back(num);
	}

	QuickSort(arr, 0, arr.size() - 1);

	mFileI[FILE_SCORES].close();
	return arr;
}

void Data::ChangeAudio(int lvl, int type)
{
	int currentLevel = 0;

	if (type == AUDIO_MUSIC) {
		currentLevel = GetAudioLevel(AUDIO_MUSIC);
		mFileO[FILE_AUDIO].open("data//MusicVolume.txt");
	}
	else if (type == AUDIO_SOUND) {
		currentLevel = GetAudioLevel(AUDIO_SOUND);
		mFileO[FILE_AUDIO].open("data//SoundVolume.txt");
	}

	// INCREASE MUSIC
	if (lvl == 1) {
		if (currentLevel == 21) {
			mFileO[FILE_AUDIO] << "Volume: " << 21 << "\n";
		}
		else {
			mFileO[FILE_AUDIO] << "Volume: " << currentLevel + 3 << "\n";
		}
	}

	// DECREASE MUSIC
	else if (lvl == -1) {
		if (currentLevel == 0) {
			mFileO[FILE_AUDIO] << "Volume: " << 0 << "\n";
		}
		else {
			mFileO[FILE_AUDIO] << "Volume: " << currentLevel - 3 << "\n";
		}
	}

	mFileO[FILE_AUDIO].close();
}

int Data::GetAudioLevel(int type)
{
	if (type == AUDIO_MUSIC) {
		mFileI[FILE_AUDIO].open("data//MusicVolume.txt");
	}
	else if (type == AUDIO_SOUND) {
		mFileI[FILE_AUDIO].open("data//SoundVolume.txt");
	}

	std::string line;
	std::getline(mFileI[FILE_AUDIO], line);

	std::size_t found = line.find_last_of(" ");
	std::string numLine = line.substr(found + 1);

	int num;
	std::stringstream ss;
	ss << numLine;
	ss >> num;

	mFileI[FILE_AUDIO].close();

	return num;
}

void Data::Swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

int Data::Partition(std::vector<int>& array, int low, int high)
{
	int pivot = array[high];
	int i = (low - 1);
	for (int j = low; j < high; j++) {
		if (array[j] <= pivot) {
			i++;
			Swap(&array[i], &array[j]);
		}
	}
	Swap(&array[i + 1], &array[high]);
	return (i + 1);
}

void Data::QuickSort(std::vector<int>& array, int low, int high)
{
	if (low < high) {
		int pi = Partition(array, low, high);
		QuickSort(array, low, pi - 1);
		QuickSort(array, pi + 1, high);
	}
}
