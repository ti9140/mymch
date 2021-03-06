#include <fstream>

#include "functions.hpp"
#include "mymch.hpp"

int BefungeStackMachine::move(void){
	try {
		return ft[cont.cur_dir](&cont);
	}
	catch(...) {
		throw "move failed";
	}
}

long int BefungeStackMachine::getsymbol(void) {
	int new_size;
	if (cont.point[0] >= program.size()){
		if (cont.length)
			new_size = cont.length;
		else
			new_size = cont.point[0] * 1.5;
		program.resize(new_size);
	}
	if (cont.point[1] >= get_line_size(program[cont.point[0]])) {
		if (cont.length)
			new_size = cont.length;
		else
			new_size = cont.point[1] * 1.5;
		resize_line(program[cont.point[0]], new_size, static_cast<long int>(' '));
	}
	return program[cont.point[0]][cont.point[1]];
}

int BefungeStackMachine::exec(void){
	long int c = getsymbol();

	if (cont.skip) {
		cont.skip = false;
		return 0;
	}

	if (c >= '0' and c <= '9') {
		cont.push(c - '0');
		return 0;
	}

	if (cont.symb_mode && c != '"') {
		cont.push(c);
		return 0;
	}

	if (c == '>' or c == '<' or c == 'v' or c == '^') {
		cont.cur_dir = c;
		return 0;
	}

	if (c == 'p') {
		program[cont.pop()][cont.pop()] = cont.pop();
		return 0;
	}

	if (c == 'g') {
		cont.push(program[cont.pop()][cont.pop()]);
		return 0;
	}

	try {
		return ft[c](&cont);
	}
	catch(...) {
		throw "exec failed";
	}

	return 0;
}

BefungeStackMachine::~BefungeStackMachine() {};

BefungeStackMachine::BefungeStackMachine(ifstream& in, long int l = 0, long int w = 0) {
	if (l or w) {
		throw "TO_DO 25x80";
	}

	ft.insert(make_pair('>', &_move_r));
	ft.insert(make_pair('<', &_move_l));
	ft.insert(make_pair('^', &_move_u));
	ft.insert(make_pair('v', &_move_d));
	ft.insert(make_pair('+', &_add));
	ft.insert(make_pair('-', &_sub));
	ft.insert(make_pair('*', &_mult));
	ft.insert(make_pair('/', &_div));
	ft.insert(make_pair('\\', &_swap));
	ft.insert(make_pair('%', &_mod));
	ft.insert(make_pair('&', &_ask_int));
	ft.insert(make_pair('~', &_ask_char));
	ft.insert(make_pair('`', &_greater));
	ft.insert(make_pair('$', &_drop));
	ft.insert(make_pair('#', &_skip));
	ft.insert(make_pair('@', &_end));
	ft.insert(make_pair('!', &_logic_not));
	ft.insert(make_pair('|', &_v_if));
	ft.insert(make_pair('_', &_g_if));
	ft.insert(make_pair('?', &_rand_dir));
	ft.insert(make_pair('.', &_print_int));
	ft.insert(make_pair(',', &_print_char));
	ft.insert(make_pair(':', &_dup));
	ft.insert(make_pair(' ', &_void));
	ft.insert(make_pair('"', &_symbol_mode));

	srand(time(NULL));
	long int *converted_line;

	string line;
	while(getline(in, line)) {
		converted_line = new long int[line.length() + 1];
		converted_line[0] = line.length();
		converted_line = &converted_line[1];
		for(unsigned int i = 0; i < line.length(); ++i)
			converted_line[i] = line[i];
		program.push_back(converted_line);
	}
}


void BefungeStackMachine::print_program() {
	for (auto line: program)
		cout << line << endl;
}

void BefungeStackMachine::run(PP&& start_point){
	cont.running = true;
	std::swap(cont.point, start_point);
	do{
		if (exec() or move())
			cont.running = false;
	} while(cont.running);
}

int main(int args, char* argv[]){
	if (args != 2) {
		cout << "invalid param" << endl;
		exit(0);
	}
	ifstream in( argv[1] );
	BefungeStackMachine* A = new BefungeStackMachine(in);
	PP a = {0,0};
	A->run(std::move(a));
	delete(A);
	in.close();
	return 0;
}
