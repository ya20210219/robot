#include "main.h"
#include "renderer.h"
#include "polygon2D.h"

D3DXVECTOR3 pos;
D3DXVECTOR3 pos1;

void Polygon2D::Init()
{
	pos.x = 400.0f;
	pos.y = 100.0f;
	pos.z = 0.0f;

	VERTEX_3D vertex[4];	//頂点を作るための処理

	vertex[0].Position = D3DXVECTOR3(pos.x - 400.0f, pos.y - 100.0f, pos.z);			//座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//法線ベクトル
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルカラー
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	vertex[1].Position = D3DXVECTOR3(pos.x + 400.0f, pos.y - 100.0f, pos.z);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(pos.x - 400.0f, pos.y + 100.0f, pos.z);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(pos.x + 400.0f, pos.y + 100.0f, pos.z);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
	
	//頂点バッファの生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;	//頂点バッファのサイズ(4頂点分のバイト数)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//これが頂点バッファであるという証明
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer);

	//テクスチャの読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
										   "asset/texture/TutorialTex.jpg",
										   NULL,
										   NULL,
										   &m_Texture,
										   NULL);

	assert(m_Texture);	//textureの読み込みが失敗した場合エラーを出す仕組み

	//コンパイル済みのシェーダを読み込んでいる
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	//コンパイル済みのシェーダを読み込んでpixelshaderを作っている
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

}

void Polygon2D::UnInit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Polygon2D::Update()
{
	//ポリゴンを動かす場合はここで処理する
	//pos.x += 1.0f;
	//pos1.x += 0.1f;
}

void Polygon2D::Draw()
{
	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(pos.x - 400.0f, pos.y - 100.0f, pos.z);			//座標
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//法線ベクトル
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	//マテリアルカラー
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				//テクスチャ座標

	vertex[1].Position = D3DXVECTOR3(pos.x + 400.0f, pos.y - 100.0f, pos.z);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(pos.x - 400.0f, pos.y + 100.0f, pos.z);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(pos.x + 400.0f, pos.y + 100.0f, pos.z);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);

	//ID3D11DeviceContext* deviceContext = Renderer::GetDeviceContext();
	//deviceContext->Draw(4, 0);

}
