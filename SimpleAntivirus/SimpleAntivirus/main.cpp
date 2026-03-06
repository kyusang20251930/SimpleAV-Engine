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

	char* fileBuffer = new char[fileSize];

	if (ReadFile(hFile, fileBuffer, fileSize, &bytesRead, NULL)) {
		std::cout << "파일 전체를 메모리에 로드 했습니다. (" << bytesRead << " bytes)" << std::endl;
		//이제 fileBuffer[0] 부터 fileBufer[fileSize-1] 까지 모든 데이터를 검사 할 수 있다.

		//전체를 읽었으니 fileBuffer의 0번, 1번 인덱스를 확인한다.
		if (bytesRead >= 2 && fileBuffer[0] == 'M' && fileBuffer[1] == 'Z') {
			std::cout << "확인 결과 : 이 파일은 유효한 실행 파일 (MZ)입니다." << std::endl;
		}
		else {
			std::cout << "경고 : 실행 파일(MZ) 형식이 아닙니다." << std::endl;
		}
	}

	//다 사용했으면 반납
	delete[] fileBuffer;

	//파일 핸들 닫기 (닫아줘야 메모리 누수가 없다.)
	CloseHandle(hFile);
	
	return 0;
}


