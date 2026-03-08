#include<windows.h> //윈도우 운영체제의 기능을 쓰기 위한 윈도우 전용 도구 상자
#include<iostream> //C++ 의 기본 도구 상자, 화면 글자를 출력하거나 입력을 받을 때 사용
#include<wincrypt.h> //암호 도구
#include<iostream>
#include<iomanip> //16진수 출력을 위해 필요

void PrintMD5(BYTE* payload, DWORD payloadSize) {
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	BYTE rgbHash[16]; //MD5는 항상 16바이트(128비트)이다.
	DWORD cbHash = 16;

	//암호 서비스 제공자(CSP) 연결
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {

		//MD5 해시 객체 생성
		if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {

			//데이터 주입
			if (CryptHashData(hHash, payload, payloadSize, 0)) {

				//결과값 가져오기
				if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
					std::cout << "MD5 Hash: ";
					for (DWORD i=0; i < cbHash; i++) {
						//16진수로 출력
						std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)rgbHash[i];
					}
					std::cout << std::dec << std::endl;
				}
			}
			CryptDestroyHash(hHash);
		}
		CryptReleaseContext(hProv, 0);
	}
}

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

		PrintMD5((BYTE*)fileBuffer, bytesRead);
	}

	//다 사용했으면 반납
	delete[] fileBuffer;

	//파일 핸들 닫기 (닫아줘야 메모리 누수가 없다.)
	CloseHandle(hFile);
	
	return 0;
}


