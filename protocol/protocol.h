#pragma once

#include "../common.h"

#define MAX_NAME_LEN 128
#define MAX_VALUE_LEN 512
#define CHUNK_FIELDS_COUNT 12

typedef struct field_name {
    uint32_t len;
    uint8_t value[MAX_NAME_LEN];
} field_name_t;

typedef struct field_value {
    uint32_t len;
    uint8_t value[MAX_VALUE_LEN];
} field_value_t;

typedef struct field {
    field_name_t name;
    field_value_t value;
} field_t;

typedef struct msg {
    int8_t byte_order;
    int8_t message_len;
    int8_t current_field_idx;
    field_t *fields;
    int32_t fields_count;
} message_t;

#include <stdio.h>

/*
    message_t msg;
    init_message(&msg);
        add_field("key", "value");
        add_field("key1", "value1");
        add_field("key2", "value2");
    msg_free(&msg);
*/

int8_t msg_alloc (fields_t *fields, int8_t fields_count) {
    fields = calloc(fields_count, sizeof(field_t));
    if (!fields)
        return 0;
    return 1;
}

int8_t msg_init (message_t *msg) {
    if (!msg_alloc(msg->fields, CHUNK_FIELDS_COUNT))
        return 0;

    msg->byte_order = BYTE_ORDER;
    msg->current_field_idx = 0;
    msg->fields_count = CHUNK_FIELDS_COUNT;
    return 1;
}

int8_t msg_realloc (message_t *msg) {
    field_t *new_fields = msg_alloc(new_fields, CHUNK_FIELDS_COUNT*2);
    if (!new_fields)
        return 0;

    int8_t idx = msg->current_field_idx;
    for(; idx > 0; idx--) {
        new_fields[idx] = msg->fields[idx];
    }

    free(msg->fields);
    msg->fields = new_fields;
    return 1;
}

int8_t msg_add_field (message_t *msg, const char *name, const char value) {
    if (!msg->fields)
        return 0;

    if (msg->current_field_idx == msg->fields_count) {
        if (!msg_realloc(msg))
            return 0;
    }

    field_t *field = &msg->fields[msg->current_field_idx];

    {
        int32_t len = strlen(name);
        len = len <= MAX_NAME_LEN ? len : MAX_NAME_LEN;
        strncpy(field->name.value, name, len);
        field->name.len = len;
        msg->message_len += len;
    }

    {
        int32_t len = strlen(name);
        len = len <= MAX_NAME_LEN ? len : MAX_VALUE_LEN;
        strncpy(field->value.value, name, len);
        field->value.len = len;
        msg->message_len += len;
    }

    msg->current_field_idx++;

    return 1;
}

int8_t msg_free (message_t *msg) {
    if (msg->fields) free(msg->fields);
    msg->fields = NULL;
    return 1;
}

int8_t msg_pack (message_t *msg, buffer_t *buf) {
    buf->len =
            sizeof(msg->byte_order) + /* byte */
            sizeof(msg->message_len) + /* message len */
            sizeof(msg->fields_count) + /*fields count */
            msg->fields_count * sizeof(field_t); /* payload */

    buf->buf = calloc(sizeof(uint8_t), buf->len);

    if (!buf->buf)
        return 0;

    uint8_t *ptr8 = buf->buf;
    *ptr8 = msg->byte_order;
    ptr8++;

    uint32_t *ptr32 = ptr8;
    *ptr32 = message_len; ptr32++;
    *ptr32 = fields_count; ptr32++;
    ptr8 = ptr32;
    
}

int8_t psg_unpack (buffer_t *buf, message_t *msg) {

}

