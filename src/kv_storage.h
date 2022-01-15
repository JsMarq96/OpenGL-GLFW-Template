#ifndef KV_STORAGE_H_
#define KV_STORAGE_H_

//
// Created by jsmar on 12/06/2021.
//

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <cassert>

//**
// A simple key value storage utility using a Radix tree
// by Juan S. Marquerie
// TODO: Cleanup and dealloc
// */

inline unsigned int MIN(unsigned int x, unsigned int y) { return (x < y) ? x : y; }

// =================
//  UTILITIES
// =================

inline int string_similarity(const char* str1,
                             const unsigned int str1_len,
                             const char* str2,
                             const unsigned int str2_len) {
    int count = 0;
    unsigned int min_len = MIN(str1_len, str2_len);
    for(; min_len > count ; str1++, str2++) {
        if (*str1 == *str2) {
            count++;
        } else {
            break;
        }
    }
    return count;
}

// Union that stores the info of the nodes
union uKVStorage {
    char str[30];
    int integer;
    float floating_point;
};

struct sRadNode {
    char       key[32]          = "";
    int        key_len          = 0;
    uKVStorage result;
    bool       has_result     = false;

    sRadNode *children[256] = { NULL };
    bool is_full[256]         = { false };
};

inline void RN_init(sRadNode *node) {
    memset(node->key, '\0', sizeof(sRadNode::key));
    memset(node->is_full, false, sizeof(sRadNode::is_full));
    node->key_len        = 0;
    node->has_result     = false;

    for(int i = 0; i < 256; i++) {
        node->children[i] = NULL;
    }
}

inline void RN_clean(sRadNode *node) {
    for(int i = 0; i < 256; i++){
        if (node->is_full[i]) {
            RN_clean(node->children[i]);
            free(node->children[i]);
        }
    }
}

inline bool RadNode_is_leaf(sRadNode *node) {
    return node->has_result;
}

inline bool Rad_Node_get(sRadNode *node,
                        const char *key,
                        const int key_len,
                        uKVStorage *to_retrieve) {
    int index = (int) *key;

    if (node == NULL) {
        return false;
    }

    // Early stop
    if (!node->is_full[index]) {
        return false;
    }

    sRadNode *it_node = node->children[index];

    // Traverse the tree until a leaf is found
    char *res_key = (char*) key;
    int res_key_len = key_len;
    while(true) {
        int similarity = string_similarity(it_node->key, it_node->key_len, res_key, res_key_len);
        // Found the node, exiting the loop
        if (similarity == res_key_len && similarity == it_node->key_len) {
            break;
        }

        res_key += similarity;
        res_key_len -= similarity;

        // In there is no match for the keys, or the next iteration is empty, early exit
        if (similarity < it_node->key_len || !it_node->is_full[*res_key]) {
            return false;
        }

        it_node = it_node->children[*res_key];
    }
    if (!it_node->has_result) {
        return false;
    }
    memcpy(to_retrieve, &it_node->result, sizeof(uKVStorage));
    return true;
}

// TODO: Malloc on here might be screwing oerformance on the future <- bottleneck ??
inline void Rad_Node_add(sRadNode *node,
                         const char *key,
                         const int key_len,
                         const uKVStorage *to_store) {
    sRadNode *new_node = (sRadNode*) malloc(sizeof(sRadNode));
    RN_init(new_node);
    memcpy(&new_node->result, to_store, sizeof(uKVStorage));
    new_node->has_result = true;

    int node_key = (unsigned char) key[0];
    if (!node->is_full[node_key]) {
        // Early out
        new_node->key_len = key_len;
        memcpy(new_node->key, key, key_len);
        node->is_full[node_key] = true;
        node->children[node_key] = new_node;
        return;
    }

    sRadNode* it_node = node->children[node_key];
    char it_key[32] = "";
    char crop_it_key[32] = "";
    memcpy(it_key, key, key_len * sizeof(char));
    int it_key_len = key_len;

    while(true) {
        unsigned int similarity = string_similarity(it_node->key,
                                                    it_node->key_len,
                                                    it_key,
                                                    it_key_len);

        int crop_it_key_index =  (unsigned char) it_key[similarity];

        // Split the node by the similarity, generating a new child, with the old values
        if (similarity < it_node->key_len) {
            sRadNode *old_root = (sRadNode*) malloc(sizeof(sRadNode));
            memcpy(old_root, it_node, sizeof(sRadNode));
            old_root->key_len -= similarity;
            memcpy(old_root->key, it_node->key + similarity, old_root->key_len * sizeof(char));

            memset(it_node->is_full, false, sizeof(sRadNode::is_full));
            it_node->key_len = similarity;
            //memcpy(it_node->key, it_node->key, it_node->key_len);
            it_node->has_result = false;
            it_node->is_full[(unsigned char) old_root->key[0]] = true;
            it_node->children[(unsigned char) old_root->key[0]] = old_root;
        }


        // If the current node is equal to the key, then store
        // the result in the current node
        if (similarity == it_key_len && it_node->key_len == similarity) {
            memcpy(&it_node->result, to_store, sizeof(uKVStorage));
            it_node->has_result = true;
            free(new_node);
            return;
        }

        // update the iterating key
        it_key_len -= similarity;
        memcpy(it_key, it_key + similarity, it_key_len);

        // If the next node is empty, insert
        if (!it_node->is_full[crop_it_key_index]) {
            // Add new node
            new_node->key_len = it_key_len;
            memcpy(new_node->key, it_key, it_key_len * sizeof(char));
            it_node->is_full[crop_it_key_index] = true;
            it_node->children[crop_it_key_index] = new_node;
            return;
        }

        // If its not empy, go to the next node
        it_node = it_node->children[crop_it_key_index];
    }
}

struct sKVStorage {
    sRadNode  *root_node;

    inline void init() {
        root_node = (sRadNode*) malloc(sizeof(sRadNode));
        RN_init(root_node);
    }

    inline void clean() {
        RN_clean(root_node);
        free(root_node);
    }

    inline void add(const char *key,
                    const int key_len,
                    const int to_store) {

        uKVStorage result{};
        result.integer = to_store;
        Rad_Node_add(root_node,
                     key,
                     key_len,
                     &result);
    }

    inline void add(const char *key,
                    const int key_len,
                    const float to_store) {
        uKVStorage result{};
        result.floating_point = to_store;
        Rad_Node_add(root_node,
                     key,
                     key_len,
                     &result);
    }

    inline void add(const char *key,
                    const int key_len,
                    const char to_store[30]) {
        uKVStorage result{};
        // This is kinda yuck yuck, but since both of them are only allowed to go out of 30 chars...
        // should be fine right..? (no, but will fix it later)
        strcpy(&result.str[0], &to_store[0]);
        Rad_Node_add(root_node,
                     key,
                     key_len,
                     &result);
    }

    inline int get_int(const char* key,
                       const int key_len) {
        uKVStorage result;
        bool success = Rad_Node_get(root_node,
                                    key,
                                    key_len,
                                    &result);

        return (success) ? result.integer : -1;
    }

    inline float get_float(const char* key,
                           const int key_len) {
        uKVStorage result;
        bool success = Rad_Node_get(root_node,
                                    key,
                                    key_len,
                                    &result);

        return (success) ? result.floating_point : -1;
    }

};

#endif // KV_STORAGE_H_
