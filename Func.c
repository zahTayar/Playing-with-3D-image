#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "Struct.h"
#include "func.h"

//Authors :Zah Tayar Id :203133681 
//***************--------------***************



//remove space and put between them ','
char* removeSpaces(char* a) {
	int i = 0;
	int count = 0;
	char* str;
	str = (char*)malloc(100 * sizeof(char));
	str[count++] = a[i++];
	while (a[i] != '\0') {
		if (a[i] == ' ') {
			if (a[i + 1] != ' ')
				str[count++] = ',';
		}
		else
			str[count++] = a[i];
		i++;
	}
	str[count] = '\0';
	return str;
}
//in case that we recognize 'v' , deal with v- divide it to points 
Vertex* dealV(char* a) {
	Vertex* verte;
	verte = (Vertex*)malloc(sizeof(Vertex));//create struct vertex
	if (verte == NULL) {
		printf("Error when trying to malloc space ,Exiting!");
		return NULL;
	}
	verte->x = 0, verte->y = 0, verte->z = 0;//intalized
	int index = 0;
	int i = 1;
	do {
		if (a[i] == ',') {
			i++;
		}
		int count = 0;
		char str[80];
		while (a[i] != ',' && a[i] != '\0') {
			str[count++] = a[i++];
		}
		str[count] = '\0';
		if (index == 0)
			verte->x = atof(str);
		else if (index == 1)
			verte->y = atof(str);
		else verte->z = atof(str);
		index++;

	} while (a[i] != '\0');
	return verte;
}
//in case that we recognize 'f' , deal with v- divide it to points
Face* dealF(char* a) {
	Face* face;
	face = (Face*)malloc(sizeof(Face));//create struct face
	face->vertex = (int*)malloc(sizeof(int));//create array of vertex ID
	if ((face == NULL)||(face->vertex==NULL)) {
		printf("Error when trying to malloc space ,Exiting!");
		return NULL;
	}
	int size = 0;
	int i = 1;
	do {
		while (a[i] != ',' && a[i] != '\0')
			i++;
		if (a[i] == ',') {
			i++;
		}
		int count = 0;
		char str[8];
		while (a[i] != '/' && a[i] != '\0') {
			str[count++] = a[i++];
		}
		str[count] = '\0';
		if (a[i] != '\0') {
			*(face->vertex + (size)) = atoi(str);
			size++;
			face->vertex = (int*)realloc(face->vertex, (size+1) * (sizeof(int)));
		}
	} while (a[i] != '\0');
	face->size = size;
	return face;
}
//create object , and every v, and f that recognize it make realloc and bring us bigger array.
Object* createObject(char* filename) {
	Object* obj;
	obj = (Object*)malloc(sizeof(Object));
	obj->faces = (Face*)malloc(sizeof(Face));//create array of faces
	obj->vertexes = (Vertex*)malloc(sizeof(Vertex));//create array of vertexes
	if ((obj == NULL) || (obj->faces == NULL)||(obj->vertexes==NULL)) {
		printf("Error when trying to malloc space ,Exiting!");
		return NULL;
	}
	obj->numberOfFaces = 0;
	obj->numberOfVertexes = 0;
	//dealing with the file
	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		printf("failed to open the FILE .Exiting!\n");
		return 0;
	}
	do {

		char str[100];
		fgets(str, 100, f);
		if (*(str) == 'v' && *(str + 1) == ' ' ) {
			*(obj->vertexes + (obj->numberOfVertexes)) = *(dealV(removeSpaces(str)));
			obj->numberOfVertexes++;
			obj->vertexes = (Vertex*)realloc(obj->vertexes, ((obj->numberOfVertexes + 1) * (sizeof(Vertex))));
		}
		if (*(str) == 'f' && *(str + 1) == ' ') {
			*(obj->faces + (obj->numberOfFaces)) = *(dealF(removeSpaces(str)));
			obj->numberOfFaces++;
			obj->faces = (Face*)realloc(obj->faces, (obj->numberOfFaces + 1) * sizeof(Face));

		}
	} while (feof(f) == 0);
	fclose(f);
	return obj;
}
//if i will want to handle with the object in the future , this is conftrball 
// beacause of this i do create object and not read vertex and 
//change it and write to new file
void transformObject(char* originalObjectFileName, char* deformedObjectFileName) {
	Object* obj;
	int i, j;
	obj = createObject(originalObjectFileName);
	for (i = 0; i < (obj->numberOfVertexes); i++) {
		(obj->vertexes + i)->x = 0.3 * ((obj->vertexes + i)->x);
	}
	FILE* f = fopen(deformedObjectFileName, "w");
	if (f == NULL) {
		printf("failed to open the file");
		return;
	}
	for (j = 0; j < obj->numberOfVertexes; j++) {
		fprintf(f, "v   %f %f %f\n", (obj->vertexes + j)->x, (obj->vertexes + j)->y, (obj->vertexes + j)->z);
	}
	for (int i = 0; i < obj->numberOfFaces; i++) {
		fprintf(f, "f");
		for (int z = 0; z < obj->faces[i].size; z++) {
			fprintf(f," %d",obj->faces[i].vertex[z]);
		}
		fprintf(f, "\n");
	}

	fclose(f);
}


//manager of a genary func that get pointer to function and do what she need to do 
void perform(Scene* scene, void* foo(Object*, void*), char* kind, char* print) {
	ListNode* tmp = scene->list;
	int sum = 0;
	int count=0;
	int numberOfTriangularFaces=0;
	int numberOfVertexes=0;
	double totalAreaOfTriangularFaces;
	double sum1 = 0;

	if (strcmp(kind, "INT") == 0) {
		while (scene->count>count) {
			foo(tmp->value, &numberOfTriangularFaces);
			sum = sum + numberOfTriangularFaces;
			count++;
			tmp = tmp->next;
		}
		printf(print);
		printf("%d\n", sum);

	}
	else if ((strcmp(kind, "DOUBLE") == 0)) {
		while (scene->count > count) {
			foo(tmp->value, &totalAreaOfTriangularFaces);
			sum1 = sum1 + totalAreaOfTriangularFaces;
			count++;
			tmp = tmp->next;
		}
		printf(print);
		printf("%f\n", sum1);
	}
	else
		printf("something went wrong please try again.\n");
	
}
//solution of 4 A
void printFaces(Object *obj, void *numberOfTriangularFaces){
	int count=0;
	for (int i=0;i<(obj->numberOfFaces);i++){
		if((obj->faces+i)->size==3)
			count++;
	}
	 *(int*)numberOfTriangularFaces=count;
}
//solution of 4 B 
void printVertexes(Object *obj,void *numberOfVertexes){
	*((int*)numberOfVertexes)= (obj->numberOfVertexes);
}
//calculate the distance of two vertex
double calculateDist(Vertex* one, Vertex* two) {

	double distance = (pow((two->x - one->x), 2) +
		(pow((two->y - one->y), 2)) +
		(pow((two->z - one->z), 2)));
	distance = sqrt(distance);
	return distance;
}

//calculate the all area of all the object.
void getTotalArea(Object* ptr, void* totalAreaOfTriangularFaces) {
	int size = ptr->numberOfFaces;
	double totalArea = 0;
	double numOfDistances[3];
	double semiperimiter, currentArea = 0;
	
	for (int i = 0; i < size; i++) {
		if (ptr->faces[i].size == 3) {
			Vertex one = ptr->vertexes[ptr->faces[i].vertex[0] - 1];
			Vertex two = ptr->vertexes[ptr->faces[i].vertex[1] - 1];
			Vertex three = ptr->vertexes[ptr->faces[i].vertex[2] - 1];
			
			numOfDistances[0] = calculateDist(&one, &two);
			numOfDistances[1] = calculateDist(&two, &three);
			numOfDistances[2] = calculateDist(&three, &one);

			semiperimiter = 0.5 * (numOfDistances[0] + numOfDistances[1] + numOfDistances[2]);
			currentArea = sqrt(semiperimiter * (semiperimiter - numOfDistances[0]) *
				(semiperimiter - numOfDistances[1]) * (semiperimiter - numOfDistances[2]));
		
			totalArea = totalArea + currentArea;
		}
		
	}
	*(double*)totalAreaOfTriangularFaces = totalArea;
}




//save scene ,two options binar and text
void saveScene(Scene* scene, char* fileName, enum FileType type) {
	ListNode* tmp = scene->list;
	int size;
	switch (type) {

	case TextFormat:
		printf("saving in text format!  \n");
		FILE* f = fopen(fileName, "w");
		if (f == NULL) {
			printf("failed to open the file . Exiting\n");
			return NULL;
		}
		fprintf(f, "%d\n", scene->count);//write the size of the list ;
		while (tmp != NULL) {
			fprintf(f, "%d\n", (tmp->value)->numberOfVertexes);//saveing the number of vertex,and moving on the array to save each;
			for (int i = 0; i < tmp->value->numberOfVertexes; i++)
				fprintf(f, "%f %f %f\n", (tmp->value->vertexes + i)->x, (tmp->value->vertexes + i)->y, (tmp->value->vertexes + i)->z);


			//saving the number of faces,and moving on the array ,save size,
			//and then mooving on the size;
			fprintf(f, "%d\n", tmp->value->numberOfFaces);
			for (int i = 0; i < tmp->value->numberOfFaces; i++) {
				size = (tmp->value->faces + i)->size;
				fprintf(f, "%d \n", size);
				for (int j = 0; j < size; j++)
					fprintf(f, "%d ", *((tmp->value->faces + i)->vertex + j));
				fprintf(f, "\n");
			}
			tmp = tmp->next;

		}
		fclose(f);
		break;
	case BinaryFormat:
		printf("saving in binary format!  \n");
		FILE* f1 = fopen(fileName, "wb");
		if (f1 == NULL) {
			printf("failed to open the file . Exiting\n");
			return NULL;
		}
		fwrite(&scene->count, sizeof(int), 1, f1);
		while (tmp != NULL) {
			
			fwrite(&(tmp->value->numberOfVertexes), sizeof(int), 1, f1);
			fwrite(&(tmp->value->numberOfFaces), sizeof(int), 1, f1);
			fwrite((tmp->value)->vertexes, sizeof(Vertex), tmp->value->numberOfVertexes, f1);
			for (int i = 0; i < tmp->value->numberOfFaces; i++) {
				fwrite(&(tmp->value->faces[i].size), sizeof(int), 1, f1);
				fwrite((tmp->value->faces[i]).vertex, sizeof(int), (tmp->value->faces[i]).size, f1);
			}
			tmp = tmp->next;

		}
		fclose(f1);
		break;
	default:
		printf("something went wrong please try again");

	}
}

//load scene two options binar and text
Scene* loadScene(char* fileName, enum FileType type) {
	Scene* scene;
	ListNode* tmp;

	scene = (Scene*)malloc(sizeof(Scene));
	scene->list = (ListNode*)malloc(sizeof(ListNode));
	if ((scene == NULL) || (scene->list == NULL)) {
		printf("Error when trying to malloc space ,Exiting!");
		return NULL;
	}
	tmp = scene->list;

	int size, count = 0;
	switch (type) {

	case TextFormat:
		printf("loading from text format!  \n");
		FILE* f = fopen(fileName, "r");
		if (f == NULL) {
			printf("failed to open the file . Exiting\n");
			return NULL;
		}
		fscanf(f, "%d\n", &(scene->count));//read the size of the list ;
		while (count < (scene->count)) {
			
			(tmp->value) = (Object*)malloc(sizeof(Object));//set space for object
			if (tmp->value == NULL) {
				printf("Error when trying to malloc space ,Exiting!");
				return NULL;
			}
			fscanf(f, "%d\n", &tmp->value->numberOfVertexes);//reading the number of vertex,and moving on the array to read each;
			tmp->value->vertexes = (Vertex*)malloc(tmp->value->numberOfVertexes * sizeof(Vertex));//create array of vertexes
			if (tmp->value->vertexes == NULL) {
				printf("Error when trying to malloc space ,Exiting!");
				return NULL;
			}
			for (int i = 0; i < tmp->value->numberOfVertexes; i++)
				fscanf(f, "%f %f %f\n", &(tmp->value->vertexes + i)->x, &(tmp->value->vertexes + i)->y, &(tmp->value->vertexes + i)->z);


			//reading the number of faces,and moving on the array ,read size,
			//and then mooving on the size;
			fscanf(f, "%d\n", &tmp->value->numberOfFaces);
			tmp->value->faces = (Face*)malloc(tmp->value->numberOfFaces * sizeof(Face));//create array of faces
			if (tmp->value->faces == NULL) {
				printf("Error when trying to malloc space ,Exiting!");
				return NULL;
			}
			for (int z = 0; z < tmp->value->numberOfFaces; z++) {
				fscanf(f, "%d \n", &(tmp->value->faces + z)->size);
				size = (tmp->value->faces + z)->size;
				(tmp->value->faces + z)->vertex = (int*)malloc(size * sizeof(int));
				if ((tmp->value->faces + z)->vertex == NULL) {
					printf("Error when trying to malloc space ,Exiting!");
					return NULL;
				}
				for (int j = 0; j < size; j++)
					fscanf(f, "%d ", ((tmp->value->faces + z)->vertex + j));
				fscanf(f, "\n");
			}
			count++;
			tmp->next = (ListNode*)malloc(sizeof(ListNode));
			if (tmp->next == NULL) {
				printf("Error when trying to malloc space ,Exiting!");
				return NULL;
			}
			tmp = tmp->next;
			
		}
		fclose(f);
		break;
	case BinaryFormat:
		printf("loading from binary format!  \n");
		FILE* f1 = fopen(fileName, "rb");
		if (f1 == NULL) {
			printf("failed to open the file . Exiting\n");
			return NULL;
		}
		fread(&scene->count, sizeof(int), 1, f1);
		while (count < scene->count) {
			tmp->value=(Object*)malloc(sizeof(Object) );
			if (tmp->value == NULL) {
				printf("Error when trying to malloc space ,Exiting!");
				return NULL;
			}
			fread(&(tmp->value->numberOfVertexes), sizeof(int), 1, f1);
			fread(&(tmp->value->numberOfFaces), sizeof(int), 1, f1);
			tmp->value->vertexes = (Vertex*)malloc(sizeof(Vertex) * (tmp->value->numberOfVertexes));//need malloc space for vertex array
			tmp->value->faces = (Face*)malloc(sizeof(Face) * (tmp->value->numberOfFaces));//need malloc space for faces array
			if ((tmp->value->vertexes == NULL) || (tmp->value->faces == NULL)) {
				printf("Error when trying to malloc space ,Exiting!");
				return NULL;
			}
			fread(((tmp->value)->vertexes), sizeof(Vertex), tmp->value->numberOfVertexes, f1);
			for (int i = 0; i < tmp->value->numberOfFaces; i++) {
				fread(&((tmp->value->faces[i]).size), sizeof(int), 1, f1);
				tmp->value->faces[i].vertex = (int*)malloc(sizeof(int) *(tmp->value->faces[i]).size);//making space for vertex
				//array that in face
				if (tmp->value->faces[i].vertex == NULL) {
					printf("Error when trying to malloc space ,Exiting!");
					return NULL;
				}
				
				fread((tmp->value->faces[i]).vertex, sizeof(int), tmp->value->faces[i].size, f1);
			}
			
			count++;
			tmp->next = (ListNode*)malloc(sizeof(ListNode));
			tmp = tmp->next;
		}
		fclose(f1);
		break;
	default:
		printf("something went wrong please try again");
	}
	return scene;
}

//***********manage list**********************8
ListNode* createList() {//create list .
	return NULL;
}
ListNode* createNode(Object *newObj) {//create struct node
	ListNode* p = (ListNode*)malloc(sizeof(ListNode));
	p->next = NULL;
	p->value = newObj;
	return p;
}
ListNode* insertFirst(ListNode* head, Object* newObj) {
	ListNode* p = createNode(newObj);
	head = p;
	return p;
}
ListNode* insertAfter(ListNode* head, ListNode* p, Object* newObj) {
	ListNode* tmp = createNode(newObj);
	tmp->next = p->next;
	p->next = tmp;
	return head;
}
Scene* createScene(char* fileName, ...) {
	Scene* scene;
	scene = (Scene*)malloc(sizeof(Scene));
	scene->list = createList();
	ListNode* tmp;
	scene->count = 0;
	//handle the variadic func
	va_list allFiles;
	char* currentFile;

	va_start(allFiles, fileName);
	currentFile = fileName;
	scene->list = insertFirst(scene->list, (createObject(currentFile)));
	scene->count++;
	tmp = scene->list;

	while (currentFile != NULL) {
		currentFile = va_arg(allFiles, char*);
		if (currentFile == NULL)
			break;
		insertAfter(scene->list, tmp, (createObject(currentFile)));
		tmp = tmp->next;
		scene->count++;

	}

	return scene;

}
//delete alll the space that we used.
void freeScene(Scene* scene) {
	int count = 0;
	ListNode* tmp;
	ListNode* lastNode = NULL;
	tmp = scene->list;
	while (count<scene->count) {
		free(tmp->value->vertexes);
		for (int i = 0; i < tmp->value->numberOfFaces; i++) {
			free(tmp->value->faces[i].vertex);
		}
		free(tmp->value);
		free(lastNode);
		lastNode = tmp;
		tmp = tmp->next;
		count++;
	}
	free(scene);
	}
	void main() {
		Scene* scene;
		Scene* scene1, * scene2;

		// Create a Scene of Objects received 
	// from files GoingMerry.obj, Donald.obj, Pony_cartoon.obj
		scene = createScene("GoingMerry.obj.txt", "Donald.obj", "Pony_cartoon.obj.txt", NULL);

		// Print out all Vertexes
		perform(scene, printVertexes, "INT", "The number of vertexes of all objects is ");

		// Count the number of all Triangular faces of all objects in Scene 
		perform(scene, printFaces, "INT", "The number of Triangular faces of all objects is ");

		// Count the total area of all Triangular faces of all objects in Scene 
		perform(scene, getTotalArea, "DOUBLE", "The total area of Triangular faces of all objects is ");

		// Save the world to file in Text and in Binary formats
		saveScene(scene, "FirstScene.dat", TextFormat);
		saveScene(scene, "SecondScene.dat", BinaryFormat);

		// Retrieve the saved data
		scene1 = loadScene("FirstScene.dat", TextFormat);
		scene2 = loadScene("SecondScene.dat", BinaryFormat);

		// Check Retrieve the saved data of text format
		perform(scene1, printVertexes, "INT" ,"The number of vertexes of all objects is ");
		perform(scene1, printFaces, "INT", "The number of Triangular faces of all objects is ");
		perform(scene1, getTotalArea, "DOUBLE", "The total area of Triangular faces of all objects is ");

		// Check Retrieve the saved data of binary format
		perform(scene2, printVertexes, "INT", "The number of vertexes of all objects is ");
		perform(scene2, printFaces, "INT", "The number of Triangular faces of all objects is ");
		perform(scene2, getTotalArea, "DOUBLE", "The total area of Triangular faces of all objects is ");

		// Make transformation based on the original object defined in file Donald.obj
		transformObject("Donald.obj", "StrangeDonald.obj");


		// Free all memory allocated for the Scene
		freeScene(scene);
		freeScene(scene1);
		freeScene(scene2);
	
	}








