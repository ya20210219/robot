#include "main.h"
#include "renderer.h"
#include "Score.h"
#include "Scene.h"
#include "manager.h"

void Score::Init()
{
	m_Position = D3DXVECTOR3(SCORE_WIDTH + SCORE_WIDTH * MAX_SCORE_DIGIT, SCORE_WIDTH, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	VERTEX_3D vertex[4];	//���_����邽�߂̏���

	vertex[0].Position = D3DXVECTOR3(m_Position.x - SCORE_WIDTH / 2, m_Position.y - SCORE_HEIGHT / 2, 0.0f);			//���W
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�@���x�N�g��
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//�}�e���A���J���[
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//�e�N�X�`�����W

	vertex[1].Position = D3DXVECTOR3(m_Position.x + SCORE_WIDTH / 2, m_Position.y - SCORE_HEIGHT / 2, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(m_Position.x - SCORE_WIDTH / 2, m_Position.y + SCORE_HEIGHT / 2, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(m_Position.x + SCORE_WIDTH / 2, m_Position.y + SCORE_HEIGHT / 2, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@�̐���
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;	//���_�o�b�t�@�̃T�C�Y(4���_���̃o�C�g��)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//���ꂪ���_�o�b�t�@�ł���Ƃ����ؖ�
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//�e�N�X�`���̓ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/number.jpg",
		NULL,
		NULL,
		&m_Texture,
		NULL);

	assert(m_Texture);	//texture�̓ǂݍ��݂����s�����ꍇ�G���[���o���d�g��

	//�R���p�C���ς݂̃V�F�[�_��ǂݍ���ł���
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	//�R���p�C���ς݂̃V�F�[�_��ǂݍ����pixelshader������Ă���
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

}

void Score::UnInit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Score::Update()
{

}

void Score::Draw()
{
	Scene* scene = Manager::GetScene();
	int score = m_Score;
	for (int i = 0; i < MAX_SCORE_DIGIT; i++)
	{
		int num = 0;
		num = score % 10;				//�X�R�A��1�̈ʂ𔲂��o��
		score /= 10;					//�X�R�A��1�̈ʂ��炵�Ă���

		float x = num % MAX_SCORE_TEX_WIDTH * (1.0f / MAX_SCORE_TEX_WIDTH);
		float y = num / MAX_SCORE_TEX_WIDTH * (1.0f / MAX_SCORE_TEX_HEIGHT);

		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].Position = D3DXVECTOR3((m_Position.x - (SCORE_WIDTH * i)) - SCORE_WIDTH / 2, m_Position.y - SCORE_HEIGHT / 2, 0.0f);			//���W
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�@���x�N�g��
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//�}�e���A���J���[
		vertex[0].TexCoord = D3DXVECTOR2(x, y);						//�e�N�X�`�����W

		vertex[1].Position = D3DXVECTOR3((m_Position.x - (SCORE_WIDTH * i)) + SCORE_WIDTH / 2, m_Position.y - SCORE_HEIGHT / 2, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(x + (1.0f / MAX_SCORE_TEX_WIDTH), y);

		vertex[2].Position = D3DXVECTOR3((m_Position.x - (SCORE_WIDTH * i)) - SCORE_WIDTH / 2, m_Position.y + SCORE_HEIGHT / 2, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(x, y + (1.0f / MAX_SCORE_TEX_HEIGHT));

		vertex[3].Position = D3DXVECTOR3((m_Position.x - (SCORE_WIDTH * i)) + SCORE_WIDTH / 2, m_Position.y + SCORE_HEIGHT / 2, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(x + (1.0f / MAX_SCORE_TEX_WIDTH), y + (1.0f / MAX_SCORE_TEX_HEIGHT));


		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		//���̓��C�A�E�g�ݒ�
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		//�V�F�[�_�[�ݒ�
		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		//�}�g���N�X�ݒ�
		Renderer::SetWorldViewProjection2D();

		//���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		//�e�N�X�`���ݒ�
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		//�v���~�e�B�u�g�|���W�ݒ�
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//�|���S���`��
		Renderer::GetDeviceContext()->Draw(4, 0);
	}	
}