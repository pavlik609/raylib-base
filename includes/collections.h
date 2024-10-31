#include <stdio.h>
#include <stdlib.h>

#ifndef __RA_COLLECTIONS_H__
#define __RA_COLLECTIONS_H__

    /* Thanks, kind stranger! https://stackoverflow.com/a/2873874 */
    /* I hate this macro, i really do, but i aint dealin whith CPP */
    #define L_LIST_STRUC(T)  \
        typedef struct l_list_##T l_list_##T; \
        struct l_list_##T { \
            T x; \
            l_list_##T* next; \
        };

    /* Ok no, this macro is torture. Best thing is, this cant be done in a function :DDD */
    #define L_LIST_GET(T)  \
        l_list_##T* l_list_get_##T (l_list_##T* head,int n) { \
            l_list_##T* curr = head->next; \
            l_list_##T previous; \
            if(n==0){ \
                return head; \
            } \
            for(int i=1;i<n;i++){ \
                if (curr->next == NULL){ \
                    return NULL; \
                } \
                curr = curr->next; \
            } \
            return curr; \
        }

    /* At least the rest are more managable */
    #define L_LIST_PUSH(T)  \
        void l_list_push_##T (l_list_##T** head,T val) { \
            if (*head == NULL){ \
                *head = malloc(sizeof(l_list_##T)); \
                (**head).x = val; \
                return; \
            } \
            l_list_##T* oldhead = malloc(sizeof(l_list_##T)); \
            *oldhead = **head; \
            (*head)->x = val; \
            (*head)->next = oldhead; \
        }

    #define L_LIST_PUSH_END(T) \
        void l_list_push_end_##T (l_list_##T* head,T val) { \
            l_list_##T* curr = malloc(sizeof(l_list_##T)); \
            *curr = *head; \
            while(1){ \
                if(curr->next == NULL) { break; } \
                curr = curr->next; \
            } \
            curr->next = malloc(sizeof(l_list_##T)); \
            curr->next->x = val; \
        }

    /* Is it really worth it, just to avoid CPP? */
    #define L_LIST_POP_START(T)  \
        void l_list_pop_start_##T (l_list_##T* head) { \
            l_list_##T* newhead = head->next; \
            free(head); \
            head = newhead; \
        }

    #define L_LIST_POP(T) \
        void l_list_pop_##T (l_list_##T* head) { \
            l_list_##T* curr = head; \
            while(1){ \
                if(curr->next->next == NULL) { break; } \
                curr = curr->next; \
            } \
            free(curr->next); \
            curr->next = NULL; \
        }

    #define L_LIST_POP_N(T) \
        void l_list_pop_n_##T (l_list_##T** head,int n) { \
            if (head == NULL) { return; } \
            if (n == 0){ \
                l_list_##T* second = (*head)->next; \
                printf("%i\n",second == NULL); \
                free(*head); \
                *head = NULL; \
                return; \
            } \
            l_list_##T* curr = *head; \
            int i = 0; \
            while(1){ \
                if(i == n-1) { break; } \
                if(curr->next->next == NULL) { break; } \
                curr = curr->next; \
                i++; \
            } \
            l_list_##T* temp = curr->next->next; \
            free(curr->next); \
            curr->next = temp; \
        }


    #define L_LIST_READ(T) \
        void l_list_read_##T(l_list_##T* list) { \
            l_list_##T* curr = list; \
            while(1) /* dont feel like including stdbool*/ { \
                if(curr == NULL){ \
                   return; \
                } \
                l_list_debug_##T(curr->x); \
                curr = curr->next; \
            } \
        }

    #define L_LIST_PASS(T) \
        void l_list_pass_##T(l_list_##T* list,void (*foo)(T*,int)) { \
            if (list == NULL) { return; } \
            l_list_##T* curr = list; \
            int i = 0; \
            while(1) /* dont feel like including stdbool*/ { \
                if(curr == NULL){ \
                   return; \
                } \
                (*foo)(&(curr->x),i); \
                curr = curr->next; \
            } \
        }

    /* And good riddance! */
    #define L_LIST_MOD(T) \
        void l_list_mod_##T (int n,l_list_##T* head,T val) { \
            l_list_##T* toedit = l_list_get_##T(head,n); \
            if (toedit!=NULL) { \
                (*toedit).x=val; \
            } \
        }

    /* Helper function to tie in all of them */
    #define L_LIST(T) \
        L_LIST_STRUC(T) \
        L_LIST_GET(T) \
        L_LIST_PUSH(T) \
        L_LIST_PUSH_END(T) \
        L_LIST_POP_START(T) \
        L_LIST_POP(T) \
        L_LIST_POP_N(T) \
        L_LIST_MOD(T) \
        L_LIST_READ(T) \
        L_LIST_PASS(T)
/*int part1(char* in,int len){
    int retval = 0;
    int idx = 0;
    while(1){
        if(idx < len){
            retval+=((int)in[idx++]);
        }
        if(idx < len){
            retval-=((int)in[idx++]);
        }
        if(idx == len) { return retval; }
    }
}

int part2(char* in,int len){
    int retval = 0;
    int idx = len;
    while(1){
        if(idx > 0){
            retval+=((int)in[idx--]);
        }
        if(idx > 0){
            retval-=((int)in[idx--]);
        }
        if(idx == 0) { return retval; }
    }
}

int hash(char* in){
    return part1(in,strlen(in))+part2(in,strlen(in));
} TEXTURE ARRAY HASHING, PLEASE FIND A BETTER WAY */
#endif
