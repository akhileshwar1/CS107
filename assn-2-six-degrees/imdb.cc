using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "imdb.h"

const char *const imdb::kActorFileName = "actordata";
const char *const imdb::kMovieFileName = "moviedata";

imdb::imdb(const string& directory)
{
  const string actorFileName = directory + "/" + kActorFileName;
  const string movieFileName = directory + "/" + kMovieFileName;
 
  cout << actorFileName << " " << movieFileName << endl;
  actorFile = acquireFileMap(actorFileName, actorInfo);
  movieFile = acquireFileMap(movieFileName, movieInfo);
}

bool imdb::good() const
{
  return !( (actorInfo.fd == -1) || 
	    (movieInfo.fd == -1) ); 
}

// you should be implementing these two methods right here... 
bool imdb::getCredits(const string& player, vector<film>& films) const {
  // get the integer offset located at the actor file's first actor.
  int nActors =  *(int *)actorFile;
  cout << "nactors are " << nActors << endl;
  int n = 0;
  memcpy(&n, (char *)actorFile, sizeof(int));
  cout << "n is " << n << endl;
  int *offset = (int *)actorFile + 7;
  int intOffset;
  memcpy(&intOffset, offset, sizeof(int));
  cout << intOffset << " " << endl;

  char *actorOffset = (char *)actorFile + intOffset;
  char *it = actorOffset;
  int length = (int) strlen(it);
  while(*it != '\0'){
    cout << *it;
    it++;
  }

  // padded with an extra \0.
  if(length%2 == 0){
    cout << "in here " << endl;
    ++it;
  }

  cout << "length is" << length << endl;

  short movies;
  memcpy(&movies, ++it, sizeof(short));
  cout << "no of movies are " << movies << endl;
  it = it + 2; // go to the first byte following the short.

  size_t len = it - (char *)actorFile - 1; 
  cout << "length with no of movies is" << len << " " << len%4 << endl;
  // padded with two extra zeroes if the length till now was not a multiple of 4.
  if(len%4 != 0){
    it = it + 2;
  }

  int movieOffset = *(int *)it;
  cout << "movie offset is " << movieOffset << endl;

  int tMovies = *(int *)movieFile;
  cout << "movies are" << tMovies << endl;

  // let's get the movie title.
  char *movieTitle = (char *)movieFile + movieOffset;
  int titleLength = 0;
  while(*movieTitle != '\0'){
    cout << *movieTitle;
    ++movieTitle;
    ++titleLength;
  }

  return false;
}
 
bool imdb::getCast(const film& movie, vector<string>& players) const { return false; }

imdb::~imdb()
{
  releaseFileMap(actorInfo);
  releaseFileMap(movieInfo);
}

// ignore everything below... it's all UNIXy stuff in place to make a file look like
// an array of bytes in RAM.. 
const void *imdb::acquireFileMap(const string& fileName, struct fileInfo& info)
{
  struct stat stats;
  stat(fileName.c_str(), &stats);
  info.fileSize = stats.st_size;
  info.fd = open(fileName.c_str(), O_RDONLY);
  return info.fileMap = mmap(0, info.fileSize, PROT_READ, MAP_SHARED, info.fd, 0);
}

void imdb::releaseFileMap(struct fileInfo& info)
{
  if (info.fileMap != NULL) munmap((char *) info.fileMap, info.fileSize);
  if (info.fd != -1) close(info.fd);
}
