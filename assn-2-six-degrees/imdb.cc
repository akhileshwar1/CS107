using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <vector>
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

int imdb::getActorOffset(const string& player) const {
  const char *cplayer = player.c_str();
  int nActors =  *(int *)actorFile;
  int *left = (int *)actorFile + 1;
  int *right = (int *)actorFile + nActors;
  while (left <= right) {
    ptrdiff_t diff = right - left;
    int *mid = left + (diff/2);
    int cmp = strcmp(cplayer, (char *)actorFile + *mid);
    if(cmp == 0) {
      return *mid;
    }
    else if (cmp < 0) {
      right = mid - 1;
    }
    else if (cmp > 0) {
      left = mid + 1;
    }
  }

  // player not found.
  return -1;
}

// you should be implementing these two methods right here... 
bool imdb::getCredits(const string& player, vector<film>& films) const {
  // get the integer offset located at the actor file's first actor.
  int nActors =  *(int *)actorFile;
  int actorOffset = getActorOffset(player);

  if (actorOffset == -1) {
    return false;
  }

  char *it = (char *)actorFile + actorOffset;
  int length = strlen(it);
  it = it + length; // points to the null character at the end of string.

  if(length%2 == 0) {
    ++it; //padded with an extra null character.
  }

  short movies = *(short *)++it;

  it = it + 2; // go to the first byte following the short.
  size_t len = it - (char *)actorFile - 1;
  // padded with two extra zeroes if the length till now was not a multiple of 4.
  if(len%4 != 0){
    it = it + 2;
  }

  for (int i = 0; i< movies; i++) {
    int offset = *(int *)it;
    struct film f;
    char *movie = (char *)movieFile + offset;
    char *title;
    strcpy(title, movie);
    f.title = title;
    movie = movie + strlen(title);
    ++movie; // to the year.
    f.year = 1900 + (int)*movie;
    films.push_back(f);
  }

  cout << "films size is " << films.size() <<  " " << films[0].year << endl;

  return true;
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
