// ��ɾ�			�ɼ�		���� ������ ��ġ					�纻 ������ ������ ��ġ
	
xcopy			/y		.\Engine\Bin\Engine.dll			.\Client\Bin\
xcopy			/y		.\Engine\Bin\Engine.lib			.\EngineSDK\Lib\
xcopy			/y		.\Engine\ThirdPartyLib\*.lib			.\EngineSDK\Lib\

xcopy			/y/s		.\Engine\Public\*.*			.\EngineSDK\Inc\
xcopy			/y/s		.\Engine\Bin\ShaderFiles\*.*		.\EngineSDK\hlsl\

xcopy			/y		.\Engine\Bin\Engine.dll			.\MapTool\Bin\
//xcopy			/y		.\Engine\Bin\Engine.dll			.\AnimTool\Bin\
