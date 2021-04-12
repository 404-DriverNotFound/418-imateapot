1. config 파일 formatting rule - 각 라인별로 오는 cmd를 규칙을 정해서 반드시 그 규칙대로 들어오게끔 정하자
	ㄴ location block 밑에 server 관련 config 내용이 올 수 있는지?
		ㄴ 만약 올 수 있다면
		EX> server 안의 autoindex가 우선인지 / location 안의 autoindex가 우선인지?? -->
2. coding style - coding convension을 어떻게 할지.. 현재 우리는 auto로 사용하고 있는데 non-auto로 해서 coding style을 맞춰보자
3. 각 function마다 @desc, @param, @return 에 대한 간결한 설명 추가
4. webserver 에서 config 파일을 파싱하는데 굳이 여러 단계로 나누어서 파싱을 해야할까요? webserver단에서 모두 파싱이 가능하지 않을까?
	ㄴ 만약 이렇게 한다면 어떻게 코딩을 할지?
		1) pair programming
		2) 한사람이 짜기.. < 비추..
		3) 그냥 3명이 나누기...
