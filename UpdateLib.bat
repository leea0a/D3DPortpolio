// 명령어			옵션		원본 파일의 위치					사본 파일을 저장할 위치
	
xcopy			/y		.\Engine\Bin\Engine.dll			.\Client\Bin\
xcopy			/y		.\Engine\Bin\Engine.lib			.\EngineSDK\Lib\
xcopy			/y		.\Engine\ThirdPartyLib\*.lib			.\EngineSDK\Lib\

xcopy			/y/s		.\Engine\Public\*.*			.\EngineSDK\Inc\
xcopy			/y/s		.\Engine\Bin\ShaderFiles\*.*		.\EngineSDK\hlsl\

xcopy			/y		.\Engine\Bin\Engine.dll			.\MapTool\Bin\
//xcopy			/y		.\Engine\Bin\Engine.dll			.\AnimTool\Bin\
