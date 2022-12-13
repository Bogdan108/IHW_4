.PHONY: ReadWriteFile


ReadWriteFile: ./main
	./main file InOutputFiles/input.txt  InOutputFiles/output.txt
	./main file InOutputFiles/input_sec.txt  InOutputFiles/output_sec.txt
	./main file InOutputFiles/input_third.txt  InOutputFiles/output_third.txt


