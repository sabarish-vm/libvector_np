void func_f(int& x) {
    ;
}

int main() {

int a = 1;
int* b = &a;
int a2 = 2;
int* p;  // pointer to int

p = &a;  // Allowed: p stores address of a
a = a2;   // Allowed: a gets value of b
p = &a2;  // Allowed: p now points to b's address
p = a2;  // Not allowed: b is int, cannot assign int to int* pointer
a = &a2; // Not allowed: &b is int*, cannot assign pointer to int variable


func_f(&a);
// Not allowed: &a is of type int*, 
// but f expects int& (pointer cannot be passed as reference to int)

func_f(a);
// Allowed: a is int, passed by reference as expected

f(b);
// Not allowed: b is int* (pointer), but f expects int&

f(*b);
// Allowed: *b dereferences pointer b to int, passed by reference to f
}
