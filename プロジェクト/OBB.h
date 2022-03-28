#pragma once

class OBB : public GameObject
{
private:
	static Model* m_Model;
	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	D3DXVECTOR3 m_OBBPos;              // �ʒu
	D3DXVECTOR3 m_NormaDirect[3];   // �����x�N�g��
	FLOAT m_fLength[3];             // �e�������̒���

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();

	static void Load()
	{
		m_Model = new Model();
		m_Model->Load(MODEL_CUBE);	//\\2�ɒ��Ӂu//�v�ł�����
	};
	static void UnLoad()
	{
		m_Model->Unload();
		delete m_Model;
	};

	D3DXVECTOR3 GetDirect(int index) { return m_NormaDirect[index]; }   // �w�莲�ԍ��̕����x�N�g�����擾
	FLOAT GetLen_W(int index) { return m_fLength[index]; }			  // �w�莲�����̒������擾
	D3DXVECTOR3 GetPos_W() { return m_OBBPos; }				          // �ʒu���擾

	void SetOBBPos(D3DXVECTOR3 pos) { m_OBBPos = pos; }
	void SetDirect(int index, D3DXVECTOR3 dir) { m_NormaDirect[index] = dir; }
	void SetLen_W(int index, float len) { m_fLength[index] = len; }

	bool ColOBBs(OBB &obb1, OBB &obb2);

	//FLOAT LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3);
};