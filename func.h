#pragma once
char* removeSpaces(char* a);
Vertex* dealV(char* a);
Face* dealF(char* a);
Object* createObject(char* filename);
void transformObject(char* originalObjectFileName, char* deformedObjectFileName);
void perform(Scene* scene, void* foo(Object*, void*), char* kind, char* print);
void printFaces(Object* obj, void* numberOfTriangularFaces);
void printVertexes(Object* obj, void* numberOfVertexes);
double calculateDist(Vertex* one, Vertex* two);
void getTotalArea(Object* ptr, void* totalAreaOfTriangularFaces);
void saveScene(Scene* scene, char* fileName, enum FileType type);
Scene* loadScene(char* fileName, enum FileType type);
ListNode* createList();
ListNode* createNode(Object* newObj);
ListNode* insertFirst(ListNode* head, Object* newObj);
ListNode* insertAfter(ListNode* head, ListNode* p, Object* newObj);
Scene* createScene(char* fileName, ...);
void freeScene(Scene* scene);
