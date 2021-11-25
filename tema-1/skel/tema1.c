#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300
#define DEL " /\n"

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

/*
	* Functie care verifica daca in directorul dat ca parametru se va gasi un file/dir cu acel nume,
	* in cazul in care va gasi se va return true (1), else false (0). 
*/
int exists(Dir* parent, char* name) {
	
	Dir* iter_dir = parent->head_children_dirs;
	while(iter_dir) {
		if(strcmp(iter_dir->name, name) == 0) {
			return 1; //e director
		}
		iter_dir = iter_dir->next;
	}

	File* iter_files = parent->head_children_files;
	while(iter_files) {
		if(strcmp(iter_files->name, name) == 0) {
			return 2; //e fisier
		}
		iter_files = iter_files->next;
	}
	return 0; //nu a fost gasit
}

void touch (Dir* parent, char* name) {

	if(exists(parent, name)) {
		printf("File already exists\n");
		return;
	}

	Dir* iter_dir = parent->head_children_dirs;
	File* iter_files = parent->head_children_files;
	//creare fisier
	File* file = (File*)malloc(sizeof(File));
	file->name = strdup(name);
	file->parent = parent;
	file->next = NULL;

	if(parent->head_children_files == NULL) {
		parent->head_children_files = file;
		return;
	}

	iter_files = parent->head_children_files;
	File* iter_files_prev = NULL; 
	for(; iter_files != NULL;) { //vreau sa ajung la sfarsitul listei de fisiere al celulei curente(dir actual)
		iter_files_prev = iter_files;
		iter_files = iter_files->next;
	}
	iter_files_prev->next = file;
	return;
}

void mkdir (Dir* parent, char* name) {

	if(exists(parent, name)) {
		printf("Directory already exists\n");
		return;
	}

	//iterez prin direcoare si fisiere cu urm pointeri;
	Dir* iter_dir = parent->head_children_dirs;
	File* iter_files = parent->head_children_files;
	//creare director
	Dir* new_dir = (Dir*)malloc(sizeof(Dir));
	new_dir->name = strdup(name);
	new_dir->parent = parent;
	new_dir->head_children_files = NULL;
	new_dir->head_children_dirs = NULL;
	new_dir->next = NULL;

	if(parent->head_children_dirs == NULL) {
		parent->head_children_dirs = new_dir;
		return;
	}

	iter_dir = parent->head_children_dirs;
	Dir* iter_dir_prev = NULL; 
	while(iter_dir) { //vreau sa ajung la sfarsitul listei de fisiere al celulei curente(dir actual)
		iter_dir_prev = iter_dir;
		iter_dir = iter_dir->next;
	}
	iter_dir_prev->next = new_dir;
	return;
}

void ls (Dir* parent) {

	Dir* iter_dir = parent->head_children_dirs;
	while(iter_dir != NULL) {
		printf("%s\n", iter_dir->name);
		iter_dir = iter_dir->next;
	}

	File* iter_files = parent->head_children_files;
	while(iter_files != NULL) {
		printf("%s\n", iter_files->name);
		iter_files = iter_files->next;
	}
	return;
}

void free_file(File* file) {
	
	free(file->name);
	file->parent = NULL;
	file->next = NULL;
	free(file);
	return;
}

void free_dir(Dir* dir) {
	File* iter_files = dir->head_children_files;
	File* aux;
	while(iter_files) {
		aux = iter_files;
		iter_files = iter_files->next;
		free_file(aux);
	}
	dir->head_children_files = NULL;

	Dir* iter_dirs = dir->head_children_dirs;
	Dir* aux_dir;
	while(iter_dirs) {
		aux_dir = iter_dirs;
		iter_dirs = iter_dirs->next;
		free_dir(aux_dir);
	}
	
	free(dir->name);
	free(dir);

	return;
}

void rm (Dir* parent, char* name) {

	int type = exists(parent, name);
	if(type != 2) {
		printf("Could not find the file\n");
		return;
	}
	//
	File* iter_files = parent->head_children_files;
	File* iter_files_prev = NULL;
	while(iter_files) {
		if(strcmp(iter_files->name, name) == 0) {
			if(iter_files_prev == NULL) {
				parent->head_children_files = parent->head_children_files->next;
			} else {
				iter_files_prev->next = iter_files->next;
			}
			free_file(iter_files);
			return;
		}
		iter_files_prev = iter_files;
		iter_files = iter_files->next;
	}
}

void rmdir (Dir* parent, char* name) {

	int type = exists(parent, name);
	if(type != 1) {
		printf("Could not find the dir\n");
		return;
	}

	Dir* iter_dir = parent->head_children_dirs;
	Dir* iter_dir_prev = NULL;
	while(iter_dir) {
		if(strcmp(iter_dir->name, name) == 0) {
			//am gasit
			if(iter_dir_prev == NULL) { //il elimin pe singurul
				parent->head_children_dirs = parent->head_children_dirs->next;
			} else {
				iter_dir_prev->next = iter_dir->next;
			}
			free_dir(iter_dir);
			return;
		}
		iter_dir_prev = iter_dir;
		iter_dir = iter_dir->next;
	}
}

void cd(Dir** target, char *name) {

	if(strncmp(name, "..", 2) == 0) {
		//trebui sa merg in printele directoului
		if((*target)->parent == NULL){
			return;
		}
		(*target) = (*target)->parent;
		return;
	}

	Dir* iter_dir = (*target)->head_children_dirs;
	while(iter_dir) {
		if(strcmp(iter_dir->name, name) == 0) {
			(*target) = iter_dir;
			return;
		}
		iter_dir = iter_dir->next;
	}
	printf("No directories found!\n"); //poate nu exista in lista de directoare
	return;
}

/*
Functie care imi intoarce calea de la directorul radacina -> target.
Se construieste cu sprintf - main intai pun numele fisierului prefixat de /
dupa care adaug la stringul mai sus format in felul urmator, dc are parint 
in lipesc in fata stringului - si il prefixez si pe el cu /. Se va repeta pana 
cand parintele este null, deci pana cand s a ajuns in /home (la mine in root - asa am numit primul nod).
*/
char *pwd (Dir* target) {

	char* tmp = calloc(100, sizeof(char));
	char* aux; //se strica sprintf altfle, dc nu retin tmp intr-o var;

	sprintf(tmp, "/%s", target->name);
	Dir* iter_dir = target->parent;
	while(iter_dir) {
		aux = strdup(tmp);
		sprintf(tmp, "/%s%s", iter_dir->name, aux);
		iter_dir = iter_dir->parent;
		free(aux);
	}

	printf("%s\n", tmp);
	return tmp;
}

void stop (Dir* target) {

	free_dir(target);

	return;
}

void print_space(int times) {
	
	while(times) {
		printf(" ");
		times --;
	}
	return;
}

void tree (Dir* target, int level) {

	if(target == NULL) {
		return;
	}
	int file_level = level;

	Dir* iter_dir = target->head_children_dirs;
	while(iter_dir != NULL) {
		print_space(level);
		printf("%s\n", iter_dir->name);
		tree(iter_dir, level + 4);
		iter_dir = iter_dir->next;
	}

	File* iter_files = target->head_children_files;
	while(iter_files != NULL) {
		print_space(file_level);
		printf("%s\n", iter_files->name);
		iter_files = iter_files->next;
	}
	level = level + 4;
	return;
}

void mv(Dir* parent, char *oldname, char *newname) {

	int type = exists(parent, oldname);
	//File/dir nu exista in niciuna dintre situatii. Afisez mesaj corespuzator problemei.
	if(type == 0) {
		printf("File/Director not found\n");
		return;
	}
	int find_new = exists(parent, newname);
	//Dc oldname exista, vad dc newname exista, dc da nu suprascriu nume, deci ies din functie. (+afisare mesaj indicat in enuntul temei referitor la aceasta problema)
	if(find_new) { 
		printf("File/Director already exists\n");
		return;
	}
	//Exista si e un dir, deci vad unde e si in functie e asta modific
	if(type == 1) {
		Dir* iter_dir = parent->head_children_dirs;
		Dir* iter_dir_prev = NULL;
		Dir* mv_dir = NULL;
		while(iter_dir) {
			if(strcmp(iter_dir->name, oldname) == 0) {
				if(iter_dir_prev == NULL) {
					if(parent->head_children_dirs->next == NULL) { //Dc am un singur diector, ii schimb doar numele
						parent->head_children_dirs->next = NULL;
						free(parent->head_children_dirs->name);
						parent->head_children_dirs->name = strdup(newname);
						return;
					}
					//Jump peste directorul al carui nume trebuie modificat.
					parent->head_children_dirs = parent->head_children_dirs->next;
				} else {
					iter_dir_prev->next = iter_dir->next;
				}
				mv_dir = iter_dir;
			}
		iter_dir_prev = iter_dir;
		iter_dir = iter_dir->next;
		}

		mv_dir->next = NULL;
		free(mv_dir->name);
		mv_dir->name = strdup(newname);
		iter_dir_prev->next = mv_dir;
		return;
	}
	/*  
	Altfel este un fisier, in acest caz se va schimba numai numele lui altfel il caut si modific structura listei
	cel mutat se va aduga la sf listei de fisiere, ++ fac legaturile din interiorul listei de fiesiere ai sa fie in regula.
	*/
	File* iter_files = parent->head_children_files;
	File* iter_files_prev = NULL;
	File* mv_file = NULL;
	while(iter_files) {
		//vreau sa vad exact unde este in lista fisierul cu numele oldname
		if(strcmp(iter_files->name, oldname) == 0) {
			if(iter_files_prev == NULL) {
				parent->head_children_files = parent->head_children_files->next;
			} else {
				iter_files_prev->next = iter_files->next;
			}
			mv_file = iter_files;
		}
		iter_files_prev = iter_files;
		iter_files = iter_files->next;
	}
	mv_file->next = NULL;
	free(mv_file->name);
	mv_file->name = strdup(newname);
	iter_files_prev->next = mv_file;
	return;
}

int main () {
	char* line = malloc(MAX_INPUT_LINE_SIZE);
	int oprire = 1; //nu ma opresc pana cand nu primesc comada stop sau pana fgets nu citeste date nevalide
	char* word;
	char* word_mv;

	//entry point of dirs
	Dir* root = (Dir*)malloc(sizeof(Dir));
	root->name = malloc(5);
	strncpy(root->name, "home", 4);
	root->name[4] = '\0';
	root->parent = NULL;
	root->head_children_files = NULL;
	root->head_children_dirs = NULL;
	root->next = NULL;
	Dir* copy_root = root;

	do
	{
		/*
		Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/
		if(fgets(line, MAX_INPUT_LINE_SIZE, stdin) == NULL) {
			oprire = 0;
		} else {
			word = strtok(line, DEL);
			//processing command
			if(strncmp(word, "touch", 5) == 0) {
				word = strtok(NULL, DEL); //take the next token
				word[strlen(word)] = '\0';
				touch(root, word);
			}

			if(strncmp(word, "mkdir", 5) == 0) {
				word = strtok(NULL, DEL); //take the next token
				word[strlen(word)] = '\0';
				mkdir(root, word);
			}

			if(strncmp(word, "rm", 5) == 0) {
				word = strtok(NULL, DEL); //take the next token
				word[strlen(word)] = '\0';
				rm(root, word);
			}

			if(strncmp(word, "rmdir", 5) == 0) {
				word = strtok(NULL, DEL); //take the next token
				word[strlen(word)] = '\0';
				rmdir(root, word);
			}

			if(strncmp(word, "ls", 2) == 0) {
				ls(root);
			}

			if(strncmp(word, "cd", 2) == 0) {
				word = strtok(NULL, DEL); //take the next token
				word[strlen(word)] = '\0';
				cd(&root, word);
			}

			if(strncmp(word, "tree", 4) == 0) {
				tree(root, 0);
			}

			if(strncmp(word, "pwd", 3) == 0) {
				char* tmp = pwd(root);
				free(tmp);
			}

			if(strncmp(word, "mv", 2) == 0) {
				word = strtok(NULL, DEL);
				word[strlen(word)] = '\0';
				word_mv = strtok(NULL, DEL);
				word_mv[strlen(word_mv)] = '\0';
				mv(root, word, word_mv);
			}
	
			if(strncmp(word, "stop", 4) == 0) {
				stop(copy_root);
				oprire = 0;
			}
		}

	} while (oprire);
	
	free(line);

	printf("\n");
	return 0;
}
