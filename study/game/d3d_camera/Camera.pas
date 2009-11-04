// 相机类
unit Camera;

interface

uses
  Direct3D9, D3DX9;

type
  TCamera = class
  private
    FPos: TD3DXVector3;
    FRight: TD3DXVector3;
    FUp: TD3DXVector3;
    FLook: TD3DXVector3;
  public
    constructor Create(Pos: TD3DXVector3);
    // 左右移动
    procedure LeftRight(units: Single);
    // 上下移动
    procedure UpDown(units: Single);
    // 前后移动
    procedure ForBack(units: Single);
    // Right向量旋转
    procedure RotateRight(angle: Single);
    // Up向量旋转
    procedure RotateUp(angle: Single);
    // Look向量旋转
    procedure RotateLook(angle: Single);
    // 取观察矩阵
    procedure GetViewMatrix(out mat: TD3DMatrix);
    // 取位置
    procedure GetPosition(out pos: TD3DXVector3);
    // 设位置
    procedure SetPosition(const Pos: TD3DXVector3);
    // 取右向量
    procedure GetRight(out right: TD3DXVector3);
    // 取上向量
    procedure GetUp(out up: TD3DXVector3);
    // 取观察向量
    procedure GetLook(out look: TD3DXVector3);
  end;

implementation

{ TCamera }

constructor TCamera.Create(Pos: TD3DXVector3);
begin
  FPos := Pos;
  FRight := D3DXVector3(1, 0, 0);
  FUp := D3DXVector3(0, 1, 0);
  FLook := D3DXVector3(0, 0, 1);
end;

procedure TCamera.ForBack(units: Single);
begin
  FPos.x := FPos.x + FLook.x * units;
  FPos.y := FPos.y + FLook.y * units;
  FPos.z := FPos.z + FLook.z * units;
end;

procedure TCamera.GetLook(out look: TD3DXVector3);
begin
  look := FLook;
end;

procedure TCamera.GetPosition(out pos: TD3DXVector3);
begin
  pos := FPos;
end;

procedure TCamera.GetRight(out right: TD3DXVector3);
begin
  right := FRight;
end;

procedure TCamera.GetUp(out up: TD3DXVector3);
begin
  up := FUp;
end;

procedure TCamera.GetViewMatrix(out mat: TD3DMatrix);
var
  x, y, z: Single;
begin
  D3DXVec3Normalize(FLook, FLook);
  D3DXVec3Cross(FUp, FLook, FRight);
  D3DXVec3Normalize(FUp, FUp);
  D3DXVec3Cross(FRight, FUp, FLook);
  D3DXVec3Normalize(FRight, FRight);

  x := -D3DXVec3Dot(FRight, FPos);
  y := -D3DXVec3Dot(FUp, FPos);
  z := -D3DXVec3Dot(FLook, FPos);

  mat._11 := FRight.x; mat._12 := FUp.x; mat._13 := FLook.x; mat._14 := 0;
  mat._21 := FRight.y; mat._22 := FUp.y; mat._23 := FLook.y; mat._24 := 0;
  mat._31 := FRight.z; mat._32 := FUp.z; mat._33 := FLook.z; mat._34 := 0;
  mat._41 := x;        mat._42 := y;     mat._43 := z;       mat._44 := 1;
end;

procedure TCamera.LeftRight(units: Single);
begin
  FPos.x := FPos.x + FRight.x * units;
  FPos.y := FPos.y + FRight.y * units;
  FPos.z := FPos.z + FRight.z * units;
end;

procedure TCamera.RotateLook(angle: Single);
var
  M: TD3DXMatrix;
begin
  D3DXMatrixRotationAxis(M, FLook, angle);
  D3DXVec3TransformCoord(FUp, FUp, M);
  D3DXVec3TransformCoord(FRight, FRight, M);
end;

procedure TCamera.RotateRight(angle: Single);
var
  M: TD3DXMatrix;
begin
  // 先计算出绕FRight旋转的变换矩阵
  D3DXMatrixRotationAxis(M, FRight, angle);
  // 再将Up和Look变换坐标
  D3DXVec3TransformCoord(FUp, FUp, M);
  D3DXVec3TransformCoord(FLook, FLook, M);
end;

procedure TCamera.RotateUp(angle: Single);
var
  M: TD3DXMatrix;
begin
  D3DXMatrixRotationAxis(M, FUp, angle);
  D3DXVec3TransformCoord(FRight, FRight, M);
  D3DXVec3TransformCoord(FLook, FLook, M);
end;

procedure TCamera.SetPosition(const Pos: TD3DXVector3);
begin
  FPos := pos;
end;

procedure TCamera.UpDown(units: Single);
begin
  FPos.x := FPos.x + FUp.x * units;
  FPos.y := FPos.y + FUp.y * units;
  FPos.z := FPos.z + FUp.z * units;
end;

end.
