//
// Created by tangmin on 2025/8/5.
//

/**
 * 设计思路
 * - 日志分级
 * - 高效无锁，使用每cpu记录
 * - 拆分合并，可以每个cpu单独记录日志文件，也可以将所有的cpu日志合并到同一个文件
 * - 模式切换，单独记录或统一记录
 * - 可选按时排序，日志具体内容可以按照时间进行排序，也可以选择乱序
 * - 文件大小可控，日志文件大小可设置上限
 * - 可选周期创建日志，日志记录可以每日一个文件，也可定时创建新文件比如一周
 * */

#ifndef ELOG_ELOG_H
#define ELOG_ELOG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <time.h>
#include <stdatomic.h>

#define MAX_CPUS 128
size_t ELOG_ENTRYBOX_SIZE = 1024;


enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

struct log_entry {
    uint64_t timestamp_ns;
    uint32_t cpu_id;
    uint32_t event_type;
    union {
        struct {
            char *payload;
            size_t payload_len;
        } var_msg;
        char fix_msg[256];
    };
} __attribute__((packed));

// 单个节点（链表元素）
struct log_node {
    struct log_entry *entry;
    struct log_node *next;
};

// 无锁队列（单生产者单消费者队列）
// 多个 per-CPU 实例，由 CPU 写入线程和一个落盘线程使用
struct log_queue {
    _Atomic(struct log_node *) head;  // 写入线程更新
    struct log_node *tail;            // 落盘线程更新
    pthread_mutex_t flush_lock;       // 仅用于 flush 时防止并发 flush（可选）
};

// Per-CPU buffers
static struct log_queue *cpu_buffers[MAX_CPUS];

// Writer thread control
static volatile int running = 1;




#endif //ELOG_ELOG_H
