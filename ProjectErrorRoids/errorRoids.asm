;errorRoids.asm
;A simple action shooter
;Windows32 Application 

;Include files from Irvine libraries found in the ..\Irvine folder
INCLUDE Irvine32.inc
INCLUDE GraphWin.inc

;==================== DATA =======================
.data
shipLocX dword 50	;X coordinate of the ship
shipLocY dword 50   ;Y coordinate of the ship
shotsFired DWORD 0  ;number of shots fired
shipPlaceholder BYTE "XXXXXX",0

PopupTitle BYTE "Weapon Fired!",0
PopupText  BYTE "PEW! "
	       BYTE "PEW!",0

GreetTitle BYTE "ErrorRoids!",0
GreetText  BYTE "Welcome to ErrorRoids! "
	       BYTE "Press OK to begin. ",0

CloseMsg   BYTE "Thank you for playing!",0

ErrorTitle  BYTE "Error",0
WindowName  BYTE "ErrorRoids!",0
className   BYTE "ErrorRoids ASMWin",0

; Define the Application's Window class structure.
MainWin WNDCLASS <NULL,WinProc,NULL,NULL,NULL,NULL,NULL, \
	COLOR_WINDOW,NULL,className>

msg	      MSGStruct <>
winRect   RECT <>
hMainWnd  DWORD ?
hInstance DWORD ?

;=================== CODE =========================
.code
WinMain PROC
; Get a handle to the current process.
	INVOKE GetModuleHandle, NULL
	mov hInstance, eax
	mov MainWin.hInstance, eax

; Load the program's icon and cursor.
	INVOKE LoadIcon, NULL, IDI_APPLICATION
	mov MainWin.hIcon, eax
	INVOKE LoadCursor, NULL, IDC_ARROW
	mov MainWin.hCursor, eax

; Register the window class.
	INVOKE RegisterClass, ADDR MainWin
	.IF eax == 0
	  call ErrorHandler
	  jmp Exit_Program
	.ENDIF

; Create the application's main window.
; Returns a handle to the main window in EAX.
	INVOKE CreateWindowEx, 0, ADDR className,
	  ADDR WindowName,MAIN_WINDOW_STYLE,
	  CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
	  CW_USEDEFAULT,NULL,NULL,hInstance,NULL
	mov hMainWnd,eax

; If CreateWindowEx failed, display a message & exit.
	.IF eax == 0
	  call ErrorHandler
	  jmp  Exit_Program
	.ENDIF

; Show and draw the window.
	INVOKE ShowWindow, hMainWnd, SW_SHOW
	INVOKE UpdateWindow, hMainWnd

; Display a greeting message.
	INVOKE MessageBox, hMainWnd, ADDR GreetText,
	  ADDR GreetTitle, MB_OK

; Begin the program's message-handling loop.
Message_Loop:
	; Get next message from the queue.
	INVOKE GetMessage, ADDR msg, NULL,NULL,NULL

	;INVOKE Paint, hMainWnd, shipLocX, shipLocY, ADDR shipPlaceholder

	; Quit if no more messages.
	.IF eax == 0
	  jmp Exit_Program
	.ENDIF

	; Relay the message to the program's WinProc.
	INVOKE DispatchMessage, ADDR msg
    jmp Message_Loop

Exit_Program:
       ;debug out
       mov eax,shipLocX
	  Call WriteDec
	  Call CRLF
	  MOV eax,shipLocY
	  Call WriteDec
	  Call CRLF
	  MOV eax,shotsFired
	  call WriteDec
	  Call CRLF

	  INVOKE ExitProcess,0
WinMain ENDP

;-----------------------------------------------------
WinProc PROC,
	hWnd:DWORD, localMsg:DWORD, wParam:DWORD, lParam:DWORD
; The application's message handler, which handles
; application-specific messages. All other messages
; are forwarded to the default Windows message
; handler.
;-----------------------------------------------------
	mov eax, localMsg

	.IF eax == WM_LBUTTONDOWN		; mouse button?
	  INVOKE MessageBox, hWnd, ADDR PopupText,
	    ADDR PopupTitle, MB_OK
	  inc shotsFired			;increase shots fired
	  jmp WinProcExit
	.ELSEIF eax == WM_CREATE		; create window?
	  
	  jmp WinProcExit
	.ELSEIF eax == WM_CLOSE		; close window?
	  INVOKE MessageBox, hWnd, ADDR CloseMsg,
	    ADDR WindowName, MB_OK
	  INVOKE PostQuitMessage,0
	  jmp WinProcExit
	.ELSEIF eax == WM_KEYDOWN     ; keyboard controls
	  ;jump table to find virtual key from wparam
	  mov eax,wparam
	  cmp eax,VK_UP			;up arrow
	  je UpKey
	  cmp eax,VK_DOWN			;down arrow
	  je DownKey
	  cmp eax,VK_LEFT			;left arrow
	  je LeftKey
	  cmp eax,VK_RIGHT			;right arrow
	  je RightKey
	  jmp Default
	  ; Ship movement - 3 pixels per press
	  ; Upper left corner of window is (0,0) Starting point of the ship is (50,50)
	  UpKey:
	    sub shipLocX,3
	    jmp keydownExit
       DownKey:
	    add shipLocX,3
	    jmp keydownExit
	  LeftKey:
	    sub shipLocY,3
	    jmp keydownExit
	  RightKey:
	    add shipLocY,3
	    jmp keydownExit
       Default:
      keydownExit:
	 jmp WinProcExit
	.ELSE		; other message?
	  INVOKE DefWindowProc, hWnd, localMsg, wParam, lParam
	  jmp WinProcExit
	.ENDIF

WinProcExit:
	ret
WinProc ENDP

;---------------------------------------------------
ErrorHandler PROC
; Display the appropriate system error message.
;---------------------------------------------------
.data
pErrorMsg  DWORD ?		; ptr to error message
messageID  DWORD ?
.code
	INVOKE GetLastError	; Returns message ID in EAX
	mov messageID,eax

	; Get the corresponding message string.
	INVOKE FormatMessage, FORMAT_MESSAGE_ALLOCATE_BUFFER + \
	  FORMAT_MESSAGE_FROM_SYSTEM,NULL,messageID,NULL,
	  ADDR pErrorMsg,NULL,NULL

	; Display the error message.
	INVOKE MessageBox,NULL, pErrorMsg, ADDR ErrorTitle,
	  MB_ICONERROR+MB_OK

	; Free the error message string.
	INVOKE LocalFree, pErrorMsg
	ret
ErrorHandler ENDP

;Paint PROC,
;	hWnd:DWORD, xCoord:DWORD, yCoord:DWORD, toDraw:PTR BYTE

;	ret
;Paint endP

END WinMain
