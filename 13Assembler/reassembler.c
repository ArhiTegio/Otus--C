#include <stdio.h>
#include <stdlib.h>

long data[] = {4, 8, 15, 16, 23, 42};
int data_length = sizeof(data) / sizeof(data[0]);

void print_int(long num) {
    printf("%ld ", num);
    fflush(stdout);
}

int p(long num) {
    return num & 1;
}

void add_element(long* rbp, long* rbx) {
    long* ptr = malloc(16);
    if (ptr == NULL) {
        abort();
    }
    *ptr = (long)rbp;
    *(ptr + 1) = (long)rbx;
}

void m(long* rdi, void (*rsi)(long)) {
    if (rdi == NULL) {
        return;
    }
    long* rbx = rdi;
    long* rbp = (long*)rsi;
    rdi = (long*)*rdi;
    rsi = (void (*)(long))*(rbx + 1);
    rsi(rbp);
    rdi = (long*)*(rbx + 1);
    rsi = rbp;
    m(rdi, rsi);
}

void f(long* rdi, void (*rsi)(long), void (*rdx)(long)) {
    long* rbx = rdi;
    long* r12 = (long*)rsi;
    long* r13 = (long*)rdx;
    if (rdi == NULL) {
        return;
    }
    rdi = (long*)*rdi;
    rsi = (void (*)(long))*rbx;
    rsi(r12);
    if (rax == 0) {
        add_element(rbx, r12);
        rsi = (void (*)(long))r12;
        goto ff;
    }
    rdi = (long*)*rbx;
    rsi = r12;
    f(rdi, rsi, rdx);
    goto outf;
z:
    rsi = r12;
ff:
    rdi = (long*)*(rbx + 1);
    rdx = r13;
    f(rdi, rdx, rsi);
outf:
    return;
}

int main() {
    long rax = 0;
    long rbx = data_length;
    while (rbx > 0) {
        long rdi = *(data - 1 + rbx);
        long rsi = rax;
        add_element(&rdi, &rsi);
        rbx--;
    }
    return 0;
}