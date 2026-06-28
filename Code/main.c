// Temporary test block inside your main file
int main(void)
{
    t_stack_check a = {NULL, NULL, 0, 0};
    
    // 1. Manually add nodes 10, 20, 30 using your append_node logic
    // 2. Print current sequential order: 10 -> 20 -> 30
    
    ra(&a); // Call your newly completed rotation file
    
    // 3. Print values again. If it displays 20 -> 30 -> 10, your links are safe!
    // 4. Test reverse rotation next. It should return exactly to 10 -> 20 -> 30.
}
