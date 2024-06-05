# 채팅 프로그램

서버-클라이언트 환경으로 여러 클라이언트가 동시에 채팅을 주고 받을 수 있음

##목차

-[소개]
-[설치]
-[사용법]
-[기여]
-[라이선스]
-[연락처]

##소개
채팅 프로그램은 소켓 프로그램을 이용한 프로그램이다. 서버는 클라이언트의 연결을 기다리면서 
클라이언트로부터 요청이 오면 연결을 하고 새로운 스레드를 생성한다. 이 스레드는 클라이언트의 
메시지를 다른 클라이언트에게 보내는 작업을 수행한다. 클라이언트는 처음 자신의 이름을 입력하면 
자신의 이름과 입력한 메시지가 서버에 연결된 클라이언트들에게 보내진다.

##설치

### 1. 프로젝트 복사
먼저, 프로젝트의 소스 코드를 로컬로 복사합니다.

'''bash
git clone 





