#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <limits.h>

#define MAX_ARGS 20

/* ================= init 基础 ================= */

static void setup_stdio(void)
{
    int fd = open("/dev/console", O_RDWR);
    if (fd < 0)
        _exit(1);
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2)
        close(fd);
}

static void early_mounts(void)
{
    mkdir("/proc", 0555);
    mkdir("/sys", 0555);
    mount("proc", "/proc", "proc", 0, NULL);
    mount("sysfs", "/sys", "sysfs", 0, NULL);
}

/* ================= ls 数据结构 ================= */

typedef struct {
    int show_all;
    int long_format;
    int reverse;
    int recursive;
    int classify;
    int sort_time;
} LsOptions;

typedef struct {
    char name[PATH_MAX];
    char path[PATH_MAX];
    struct stat st;
} FileInfo;

/* ================= 全局排序选项 ================= */

static LsOptions *g_sort_opt;

/* ================= 工具函数 ================= */

static void get_perm(mode_t m, char *s)
{
    int i;
    const char rwx[] = "rwx";

    strcpy(s, "----------");
    s[0] = S_ISDIR(m) ? 'd' :
           S_ISLNK(m) ? 'l' :
           S_ISCHR(m) ? 'c' :
           S_ISBLK(m) ? 'b' :
           S_ISFIFO(m) ? 'p' :
           S_ISSOCK(m) ? 's' : '-';

    for (i = 0; i < 9; i++)
        if (m & (1 << (8 - i)))
            s[i + 1] = rwx[i % 3];
}

static char classify_char(mode_t m)
{
    if (S_ISDIR(m)) return '/';
    if (S_ISLNK(m)) return '@';
    if (S_ISFIFO(m)) return '|';
    if (S_ISSOCK(m)) return '=';
    if (m & 0111) return '*';
    return 0;
}

/* ================= 排序 ================= */

static int cmp_files(const void *a, const void *b)
{
    const FileInfo *fa = (const FileInfo *)a;
    const FileInfo *fb = (const FileInfo *)b;

    if (g_sort_opt->sort_time &&
        fa->st.st_mtime != fb->st.st_mtime)
        return fa->st.st_mtime < fb->st.st_mtime ? 1 : -1;

    return strcmp(fa->name, fb->name);
}

/* ================= 输出 ================= */

static void print_long(const FileInfo *f, const LsOptions *opt)
{
    char perm[11];
    char tbuf[32];
    struct tm *tm;

    get_perm(f->st.st_mode, perm);
    tm = localtime(&f->st.st_mtime);
    strftime(tbuf, sizeof(tbuf), "%b %d %H:%M", tm);

    printf("%s %3ld %5u %5u %8ld %s %s",
           perm,
           (long)f->st.st_nlink,
           (unsigned)f->st.st_uid,
           (unsigned)f->st.st_gid,
           (long)f->st.st_size,
           tbuf,
           f->name);

    if (S_ISLNK(f->st.st_mode)) {
        char linkto[PATH_MAX];
        ssize_t n = readlink(f->path, linkto, sizeof(linkto) - 1);
        if (n > 0) {
            linkto[n] = '\0';
            printf(" -> %s", linkto);
        }
    }

    if (opt->classify) {
        char c = classify_char(f->st.st_mode);
        if (c)
            putchar(c);
    }
    putchar('\n');
}

static void print_short(const FileInfo *f, const LsOptions *opt)
{
    printf("%s", f->name);
    if (opt->classify) {
        char c = classify_char(f->st.st_mode);
        if (c)
            putchar(c);
    }
    putchar('\n');
}

/* ================= 目录列举 ================= */

static void list_dir(const char *dir, const LsOptions *opt)
{
    DIR *d;
    struct dirent *e;
    FileInfo *files = NULL;
    size_t n = 0, i;

    d = opendir(dir);
    if (!d) {
        perror(dir);
        return;
    }

    while ((e = readdir(d)) != NULL) {
        FileInfo fi;
        FileInfo *tmp;

        if (!opt->show_all && e->d_name[0] == '.')
            continue;

        snprintf(fi.name, sizeof(fi.name), "%s", e->d_name);
        snprintf(fi.path, sizeof(fi.path), "%s/%s", dir, e->d_name);

        if (lstat(fi.path, &fi.st) < 0)
            continue;

        tmp = realloc(files, (n + 1) * sizeof(FileInfo));
        if (!tmp) {
            free(files);
            closedir(d);
            return;
        }
        files = tmp;
        files[n++] = fi;
    }
    closedir(d);

    if (n == 0) {
        free(files);
        return;
    }

    g_sort_opt = (LsOptions *)opt;
    qsort(files, n, sizeof(FileInfo), cmp_files);

    if (opt->reverse)
        for (i = 0; i < n / 2; i++) {
            FileInfo t = files[i];
            files[i] = files[n - 1 - i];
            files[n - 1 - i] = t;
        }

    for (i = 0; i < n; i++)
        opt->long_format ? print_long(&files[i], opt)
                         : print_short(&files[i], opt);

    if (opt->recursive)
        for (i = 0; i < n; i++)
            if (S_ISDIR(files[i].st.st_mode) &&
                strcmp(files[i].name, ".") &&
                strcmp(files[i].name, "..")) {
                printf("\n%s:\n", files[i].path);
                list_dir(files[i].path, opt);
            }

    free(files);
}

/* ================= ls 命令 ================= */

static void shell_ls(int argc, char **argv)
{
    LsOptions opt;
    char *targets[MAX_ARGS];
    int tcnt = 0, i;

    memset(&opt, 0, sizeof(opt));

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            char *p = argv[i] + 1;
            while (*p) {
                if (*p == 'a') opt.show_all = 1;
                else if (*p == 'l') opt.long_format = 1;
                else if (*p == 'r') opt.reverse = 1;
                else if (*p == 'R') opt.recursive = 1;
                else if (*p == 'F') opt.classify = 1;
                else if (*p == 't') opt.sort_time = 1;
                p++;
            }
        } else {
            targets[tcnt++] = argv[i];
        }
    }

    if (tcnt == 0)
        targets[tcnt++] = ".";

    for (i = 0; i < tcnt; i++) {
        struct stat st;
        if (lstat(targets[i], &st) < 0) {
            perror(targets[i]);
            continue;
        }
        if (S_ISDIR(st.st_mode)) {
            if (tcnt > 1)
                printf("%s:\n", targets[i]);
            list_dir(targets[i], &opt);
            if (i + 1 < tcnt)
                putchar('\n');
        } else {
            FileInfo f;
            snprintf(f.name, sizeof(f.name), "%s", targets[i]);
            snprintf(f.path, sizeof(f.path), "%s", targets[i]);
            f.st = st;
            opt.long_format ? print_long(&f, &opt)
                            : print_short(&f, &opt);
        }
    }
}

/* ================= Shell ================= */

static void shell_loop(void)
{
    char buf[256];
    while (1) {
        char *argv[MAX_ARGS];
        int argc = 0;
        char *p;

        printf("mysh> ");
        if (!fgets(buf, sizeof(buf), stdin))
            break;

        buf[strcspn(buf, "\n")] = 0;
        if (!buf[0])
            continue;

        p = strtok(buf, " ");
        while (p && argc < MAX_ARGS - 1) {
            argv[argc++] = p;
            p = strtok(NULL, " ");
        }
        argv[argc] = NULL;

        if (!strcmp(argv[0], "ls"))
            shell_ls(argc, argv);
        else if (!strcmp(argv[0], "cd"))
            chdir(argv[1] ? argv[1] : "/");
        else if (!strcmp(argv[0], "pwd")) {
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)))
                puts(cwd);
        } else if (!strcmp(argv[0], "exit"))
            break;
        else
            puts("Unknown command");
    }
}

/* ================= main ================= */

int main(void)
{
    setup_stdio();
    early_mounts();
    puts("=== Shell start ===");
    shell_loop();
    return 0;
}
