#include<windows.h> //윈도우 운영체제의 기능을 쓰기 위한 윈도우 전용 도구 상자
#include<iostream> //C++ 의 기본 도구 상자, 화면 글자를 출력하거나 입력을 받을 때 사용

int main(void) {
	//C++ 에는 int, char 같은 기본 자료형이 있지만, windows.h를 가져오게 되면
	//윈도우가 만든 자기들 만의 특수 자료형을 사용해야 한다.

	//파일 경로
	LPCWSTR filePath = L"C:\\Windows\\System32\\notepad.exe";

	//파일 열기
	HANDLE hFile = CreateFile(
		filePath,				//경로
		GENERIC_READ,			//권한
		FILE_SHARE_READ,		//공유
		NULL,					//보안 속성
		OPEN_EXISTING,			//생성 방법
		FILE_ATTRIBUTE_NORMAL,	//속성
		NULL					//템플릿
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		std::cout << "파일을 열수 없습니다. 에러 코드 : " << GetLastError() << std::endl;
		return 1;
	}

	//파일 크기 가져오기
	DWORD fileSize = GetFileSize(hFile, NULL);
	std::cout << "파일을 성공적으로 열었습니다!" << std::endl;
	std::cout << "파일 크기: " << fileSize << " bytes" << std::endl;

	//--- ReadFile 함수 추가 ---
	char buffer[2];
	DWORD bytesRead; //실제로 읽은 바이트 수를 저장할 변수

	//2. 파일 읽기 요청
	//fFile 키를 사용해, 2바이트만 읽어 buffer에 담는다. 다 읽으면 bytesRead에 보고한다.
	if (ReadFile(hFile, buffer, 2, &bytesRead, NULL)) {

		//3. 내용 확인 (MZ 체크)
		if (bytesRead == 2 && buffer[0] == 'M' && buffer[1] == 'Z') {
			std::cout << "이 파일은 유효한 실행 파일(MZ)입니다." << std::endl;
		}
		else {
			std::cout << "실행 파일 형식이 아니거나 읽기에 실패했습니다." << std::endl;
		}
	}
	else {
		std::cout << "파일 읽기 실패. 에러 코드 : " << GetLastError() << std::endl;
	}

	//파일 핸들 닫기 (닫아줘야 메모리 누수가 없다.)
	CloseHandle(hFile);
	

	return 0;
}


