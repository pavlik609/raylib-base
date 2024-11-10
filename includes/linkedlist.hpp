#include <iostream>
#include <cstdlib>
#pragma once
using namespace std;

template <typename T>
struct linked_list {
    T value;
    linked_list* next;

};

template <typename T>
void pass(linked_list<T>** list,void (*funcptr)(linked_list<T>**,linked_list<T>*,int)){
    linked_list<T>* curr = *list;
    int i = 0;
    while(true){
        if (curr == NULL) { return; }
        funcptr(list,curr,i++);
        curr = curr->next;
    }
}

template <typename T>
void push(linked_list<T>** list,T value){
    if (*list == NULL){
        *list = (linked_list<T>*)malloc(sizeof(linked_list<T>));
        (**list).value = value;
        (**list).next = NULL;

        std::cout << "Created a new list." << std::endl;
        return;
    }
    linked_list<T>* second = (linked_list<T>*)std::malloc(sizeof(linked_list<T>));
    *second = **list;
    (**list).value = value;
    (**list).next = second;
}

template <typename T>
void passThread(linked_list<T>** list,void (*funcptr)(linked_list<T>**,linked_list<T>*,int,char**),char** update_value){
    linked_list<T>* curr = *list;
    int i = 0;
    while(true){
        if (curr == NULL) { return; }
        funcptr(list,curr,i++,update_value);
        curr = curr->next;
    }
}

template <typename T>
void pop_n(linked_list<T>** list,int n){
    if (n == 0){
        linked_list<T>* second = (*list)->next;
        free(*list);
        *list = second;
        return;
    }
    linked_list<T>* curr = *list;
    int i = 0;
    while(i<(n-1)){
        if (curr->next->next == NULL) { break; }
        curr = curr->next;
        i++;
    }
    linked_list<T>* temp = curr->next->next;

    if (curr->next == NULL) { return; }
    free(curr->next);
    
    curr->next = temp;
}
