public class Main {
	public static void main(String[] args) {
		VM vm = new VM();
		vm.run(VM.PUT, 10);
		vm.run(VM.MEMPUT);
		vm.run(VM.PUT, 0);
		vm.run(VM.MEMGET);
		vm.run(VM.PRINT);
	}
}
