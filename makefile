exql:  lexer.l parser.y
		bison -d parser.y
		flex lexer.l
		cc -c language.c
		cc -o $@ parser.tab.c language.c lex.yy.c  -ll

clean: 
	rm language 