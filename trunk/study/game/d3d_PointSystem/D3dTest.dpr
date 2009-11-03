program D3dTest;

uses
  D3dUtils in 'D3dUtils.pas',
  ParticleSystem in 'ParticleSystem.pas',
  Direct3D9, D3DX9, Windows;

var
  Snow: TSnowParticleSys;

// �����任
procedure SetupTransform;
var
  mat: TD3DMatrix;
  eye, LookAt, Up: TD3DVector;
begin
  // ���ת��
  eye := D3DXVector3(0, 0, -13);
  LookAt := D3DXVector3(0, 0, 0);
  Up := D3DXVector3(0, 1, 0);
  D3DXMatrixLookAtLH(mat, eye, LookAt, Up);
  Device9.SetTransform(D3DTS_VIEW, mat);

  // ͶӰת��
  D3DXMatrixPerspectiveFovLH(mat, D3DX_PI * 0.5, d3dpp.BackBufferWidth / d3dpp.BackBufferHeight, 1, 100);
  Device9.SetTransform(D3DTS_PROJECTION, mat);
end;

// ����֡
procedure RenderFrame(dwTime: Cardinal);
begin
  Snow.Update(dwTime * 0.001);

  // �����̨���壬 ��Ȼ���
  Device9.Clear(0, nil, D3DCLEAR_TARGET or D3DCLEAR_ZBUFFER,
    D3DCOLOR_ARGB(255, 0, 0, 0), 1, 0);

  Device9.BeginScene;
  try
    // ����ת��
    SetupTransform;

    Snow.Render;
  finally
    Device9.EndScene;
  end;

  Device9.Present(nil, nil, 0, nil);
end;


// ��ʼ��
procedure Setup;
begin
  Randomize;

  if not InitD3D(800, 600, True, D3DFMT_X8R8G8B8) then
  begin
    MessageBox(GetActiveWindow, 'D3D������ʧ�ܣ�', '', MB_OK);
    Exit;
  end;

  Snow := TSnowParticleSys.Create(0.1);
  Snow.Init(Device9, 'snowball.bmp',
    D3DXVECTOR3(-10, -10, -10),
    D3DXVECTOR3(10, 10, 10),
    1500);
end;

// ���г���
procedure Run;
begin
  MessageLoop(RenderFrame, 10);
end;

// ���
procedure Cleanup;
begin
  Snow.Free;
  TermD3D;
end;

begin
  Setup;
  Run;
  Cleanup;
end.
