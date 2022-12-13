.PHONY: ReadWriteFile


ReadWriteFile: ./main
	./main file InOutputFiles/input.txt  InOutputFiles/output.txt
	./main file InOutputFiles/input_sec.txt  InOutputFiles/output_sec.txt
	./main file InOutputFiles/input_third.txt  InOutputFiles/output_third.txt

CommandInput: ./main
	./main command 2 4:5:6:8 6:12:23:102
	./main command 4 4:5:6:8 6:23:13:1012 14:15:6:108 6:12:23:1002 

RandomInput: ./main
	./main random
	./main random