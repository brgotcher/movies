#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>


struct movie {
    char *title;
    char *year;
    char *languages;
    char *rating;
	struct movie* next;
};

struct movie* createMovie(char *curLine) {
	struct movie *curMovie = malloc(sizeof(struct movie));

	char *saveptr;
	// get title
	char *token = strtok_r(curLine, ",", &saveptr);
	curMovie->title = calloc(strlen(token) + 1, sizeof(char));
	strcpy(curMovie->title, token);

	// get year
	token = strtok_r(NULL, ",", &saveptr);
	curMovie->year = calloc(strlen(token)+1, sizeof(char));
	strcpy(curMovie->year, token);

	// get Languages
	token = strtok_r(NULL, ",", &saveptr);
	curMovie->languages = calloc(strlen(token)+1, sizeof(char));
	strcpy(curMovie->languages, token);

	// get rating Value
	token = strtok_r(NULL, "\n", &saveptr);
	curMovie->rating = calloc(strlen(token)+1, sizeof(char));
	strcpy(curMovie->rating, token);

	curMovie->next = NULL;

	return curMovie;
}

struct movie *processFile(char *filePath) {
	FILE *movieFile = fopen(filePath, "r");

	char *curLine = NULL;
	size_t len = 0;
	size_t nread;
	char *token;

	struct movie *head = NULL;
	struct movie *tail = NULL;

	nread = getline(&curLine, &len, movieFile);

	while ((nread = getline(&curLine, &len, movieFile)) != -1) {
		struct movie *newNode = createMovie(curLine);
		if (head == NULL) {
			head = newNode;
			tail = newNode;
		} else {
			tail->next = newNode;
			tail = newNode;
		}
	}
	free(curLine);
	fclose(movieFile);
	return head;
}

void printMovie(struct movie* aMovie) {
	printf("Title: %s\nYear: %s\nLanguages: %s\nRating: %s \n", aMovie->title, aMovie->year, aMovie->languages, aMovie->rating);
}

void printMovieList(struct movie *list) {
	printf("running printMovieListr");
	while (list != NULL) {
		printMovie(list);
		list = list->next;
	}
}

int countMovies(struct movie *list) {
	int count = 0;
	while (list != NULL) {
		count += 1;
		list = list->next;
	}
	return count;
}

void moviesInYear(struct movie *list) {
	int year;
	struct movie *head = NULL;
	struct movie *tail = NULL;
	printf("Enter the year: \n");
	scanf("%d", &year);
	while (list != NULL) {
		//printf("list item: %s\n", list->title);
		if (atoi(list->year) == year) {
			struct movie *newMovie = malloc(sizeof(struct movie));
			memcpy(newMovie, list, sizeof list);
			newMovie->next = NULL;
			if (head == NULL) {
				head = newMovie;
				tail = head;
				//printf("NEW HEAD: %s\n", head->title);
			} else {
				tail->next = newMovie;
				tail = newMovie;
				//printf("ADDED TO LIST: %s\n", newMovie->title);
			}
		}
		list = list->next;
	}
	while (head != NULL) {
		printf("%s\n", head->title);
		head = head->next;
	}
}

void highestRated(struct movie *list) {
	struct movie *arr[121] = {NULL};
	while (list != NULL) {
		int year = atoi(list->year);
		//printf("Year: %d\n", year);
		int index = year - 1900;
		if (arr[index] == NULL) {
			arr[index] = list;
			printf("Added %s to year %d\n", arr[index]->title, year);
		} else {
			if (strtod(arr[index]->rating, NULL) < strtod(list->rating, NULL)) {
				arr[index] = list;
			}
		}
		list = list->next;
	}
	for (int i = 0; i < 121; i += 1) {
		if (arr[i] != NULL) {

			printf("%i %s %s\n", atoi(arr[i]->year), arr[i]->rating, arr[i]->title);
		}
	}
}

void moviesByLanguage() {
	printf("moviesByLanguage");
}

int main() {

	char fileName[] = "movies_sample_1.csv";
	struct movie *list = processFile(fileName);
	int count = countMovies(list);
	printf("Processed file %s and parsed data for %d movies\n", fileName, count);
	//printMovieList(list);
	int selection;
	while (selection != 4) {
		printf("Select an option: \n");
		printf("1. show movies released in the specified year\n");
		printf("2. Show highest rated movie for each year\n");
		printf("3. Show the title and year of release of all movies in a specific language\n");
		printf("4. Exit from the program\n");
		scanf("%d", &selection);
		if (selection == 1) {
			moviesInYear(list);
		} else if (selection == 2) {
			highestRated(list);
		} else if (selection == 3) {
			moviesByLanguage();
		} else if (selection == 4) {
			break;
		} else {
			printf("invalid choice");
		}
	}



    return 0;
}