#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include<random>

float Radian(float x) {
	return x * 3.14f / 180.0f;
}

float Do(float x) {
	return x * 180 / 3.14;
}

float Minnum(float num, float num2) {
	if (num < num2) {
		return num;
	}
	return num2;
}

float Maxnum(float num, float num2) {
	if (num > num2) {
		return num;
	}
	return num2;
}

float Clamp(float min, float max, float num) {
	if (Minnum(min, num) == min) {
		return min;
	}
	else if (Maxnum(max, num) == max) {
		return max;
	}
	return num;
}

Matrix4 matScale(WorldTransform worldTransform) {
	Matrix4 matScale;
	matScale.m[0][0] = worldTransform.scale_.x;
	matScale.m[1][1] = worldTransform.scale_.y;
	matScale.m[2][2] = worldTransform.scale_.z;
	matScale.m[3][3] = 1;
	return matScale;
}
Matrix4 matRotZ(WorldTransform worldTransform) {
	Matrix4 matRotZ;
	matRotZ.m[0][0] = cos(worldTransform.rotation_.z);
	matRotZ.m[0][1] = sin(worldTransform.rotation_.z);
	matRotZ.m[1][0] = -sin(worldTransform.rotation_.z);
	matRotZ.m[1][1] = cos(worldTransform.rotation_.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;
	return matRotZ;
}

Matrix4 matRotX(WorldTransform worldTransform) {
	Matrix4 matRotX;
	matRotX.m[1][1] = cos(worldTransform.rotation_.x);
	matRotX.m[1][2] = sin(worldTransform.rotation_.x);
	matRotX.m[2][1] = -sin(worldTransform.rotation_.x);
	matRotX.m[2][2] = cos(worldTransform.rotation_.x);
	matRotX.m[0][0] = 1;
	matRotX.m[3][3] = 1;
	return matRotX;
}

Matrix4 matRotY(WorldTransform worldTransform) {
	Matrix4 matRotY;
	matRotY.m[0][0] = cos(worldTransform.rotation_.y);
	matRotY.m[2][0] = sin(worldTransform.rotation_.y);
	matRotY.m[0][2] = -sin(worldTransform.rotation_.y);
	matRotY.m[2][2] = cos(worldTransform.rotation_.y);
	matRotY.m[1][1] = 1;
	matRotY.m[3][3] = 1;
	return matRotY;
}

Matrix4 mat() {
	Matrix4 mat;
	mat.m[0][0] = 1;
	mat.m[1][1] = 1;
	mat.m[2][2] = 1;
	mat.m[3][3] = 1;
	return mat;
}

Matrix4 matTrans(WorldTransform worldTransform) {
	Matrix4 matTrans;
	matTrans.m[3][0] = worldTransform.translation_.x;
	matTrans.m[3][1] = worldTransform.translation_.y;
	matTrans.m[3][2] = worldTransform.translation_.z;
	matTrans.m[0][0] = 1;
	matTrans.m[1][1] = 1;
	matTrans.m[2][2] = 1;
	matTrans.m[3][3] = 1;
	return matTrans;
}





GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg");
	model_ = Model::Create();
	viewProjection_.Initialize();
	debugCamera_ = new DebugCamera(1600, 900);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	/*std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float> rotdist(0, 2 * 3.14f);
	std::uniform_real_distribution<float> posdist(-10.0f, 10.0f);*/


	worldTransforms_[kRoot].Initialize();
	worldTransforms_[kSpine].Initialize();
	worldTransforms_[kSpine].translation_ = { 0,4.5f,0 };
	worldTransforms_[kSpine].parent_ = &worldTransforms_[kRoot];

	worldTransforms_[kChest].Initialize();
	worldTransforms_[kChest].translation_ = { 0,0,0 };
	worldTransforms_[kChest].parent_ = &worldTransforms_[kSpine];

	worldTransforms_[kHead].Initialize();
	worldTransforms_[kHead].translation_ = { 0,2.5f,0 };
	worldTransforms_[kHead].parent_ = &worldTransforms_[kChest];
	worldTransforms_[kArmR].Initialize();
	worldTransforms_[kArmR].translation_ = { 2.5f,0,0 };
	worldTransforms_[kArmR].parent_ = &worldTransforms_[kChest];
	worldTransforms_[kArmL].Initialize();
	worldTransforms_[kArmL].translation_ = { -2.5f,0,0 };
	worldTransforms_[kArmL].parent_ = &worldTransforms_[kChest];
	worldTransforms_[kHip].Initialize();
	worldTransforms_[kHip].translation_ = { 0,-2.5f,0 };
	worldTransforms_[kHip].parent_ = &worldTransforms_[kSpine];
	worldTransforms_[kLegL].Initialize();
	worldTransforms_[kLegL].translation_ = { -2.5,-2.5,0 };
	worldTransforms_[kLegL].parent_ = &worldTransforms_[kHip];
	worldTransforms_[kLegR].Initialize();
	worldTransforms_[kLegR].translation_ = { 2.5,-2.5,0 };
	worldTransforms_[kLegR].parent_ = &worldTransforms_[kHip];

	/*worldTransform.scale_ = {1,1,1};
	worldTransform.rotation_ = { rotdist(engine),rotdist(engine),rotdist(engine) };
	worldTransform.translation_ = { posdist(engine),posdist(engine),posdist(engine) };
	worldTransform.matWorld_ = mat;
	worldTransform.matWorld_ *= matScale;
	worldTransform.matWorld_ *= matRotZ;
	worldTransform.matWorld_ *= matRotX;
	worldTransform.matWorld_ *= matRotY;
	worldTransform.matWorld_ *= matTrans;
	worldTransform.TransferMatrix();*/

	/*viewProjection_.eye = { 0,0,-10 };
	viewProjection_.target = { 10,0,0 };
	viewProjection_.up = { cosf(3.14 / 4.0f), sinf(3.14 / 4.0f), 0.0f };*/
	/*viewProjection_.fovAngleY = Radian(10.0f);*/
	/*viewProjection_.aspectRatio = 1.0f;
	viewProjection_.nearZ = 52.0f;
	viewProjection_.farZ = 53.0;
	viewProjection_.Initialize();*/
}

void GameScene::Update() {
	//debugCamera_->Update();
	//if (input_->PushKey(DIK_UP)) {
	//	/*viewProjection_.fovAngleY += 0.1f;
	//	viewProjection_.fovAngleY = Minnum(viewProjection_.fovAngleY, 3.14f);*/
	//	viewProjection_.nearZ += 1.0f;
	//}
	//else if (input_->PushKey(DIK_DOWN)) {
	//	/*viewProjection_.fovAngleY -= 0.1f;
	//	viewProjection_.fovAngleY = Maxnum(viewProjection_.fovAngleY, 0.01f);*/
	//	viewProjection_.nearZ -= 1.0f;
	//}
	//viewProjection_.UpdateMatrix();
	//debugText_->SetPos(150, 110);
	//debugText_->Printf("fovangle(Degree):%f", Do(viewProjection_.fovAngleY));
	//debugText_->SetPos(150, 130);
	//debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
	//Vector3 emove = { 0,0,0 };
	//Vector3 tmove = { 0,0,0 };
	//const float kEyeSpeed = 0.2f;
	//const float kTargetSpeed = 0.2f;
	//const float kUpRotspeed = 0.05f;
	//if (input_->PushKey(DIK_W)) {
	//	emove.z += kEyeSpeed;
	//}
	//if (input_->PushKey(DIK_S)) {
	//	emove.z -= kEyeSpeed;
	//}
	//if (input_->PushKey(DIK_LEFT)) {
	//	tmove.z += kTargetSpeed;
	//}
	//if (input_->PushKey(DIK_RIGHT)) {
	//	tmove.z -= kTargetSpeed;
	//}
	//if (input_->PushKey(DIK_SPACE)) {
	//	viewAngle += kUpRotspeed;
	//	viewAngle = fmodf(viewAngle, 3.14f * 2.0f);
	//}
	//viewProjection_.eye += emove;
	//viewProjection_.UpdateMatrix();
	//debugText_->SetPos(50, 50);
	//debugText_->Printf("eye(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	//viewProjection_.target += tmove;
	//viewProjection_.UpdateMatrix();
	//debugText_->SetPos(50, 70);
	//debugText_->Printf("target(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	//viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };
	//viewProjection_.UpdateMatrix();
	//debugText_->SetPos(50, 90);
	//debugText_->Printf("up(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	Vector3 move = { 0,0,0 };
	Vector3 Urot = { 0,0,0 };
	Vector3 Drot = { 0,0,0 };
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += 1.0f;
	}
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= 1.0f;
	}
	if (input_->PushKey(DIK_U)) {
		Urot.y += 1.0f;
	}
	if (input_->PushKey(DIK_I)) {
		Urot.y -= 1.0f;
	}
	if (input_->PushKey(DIK_J)) {
		Drot.y += 1.0f;
	}
	if (input_->PushKey(DIK_K)) {
		Drot.y -= 1.0f;
	}
	worldTransforms_[kRoot].translation_ += move;
	worldTransforms_[kHip].rotation_ += Drot;
	worldTransforms_[kChest].rotation_ += Urot;

	for (int i = 0; i < kNumPartID; i++)
	{
		worldTransforms_[i].matWorld_ = mat();
		worldTransforms_[i].matWorld_ *= matScale(worldTransforms_[i]);
		worldTransforms_[i].matWorld_ *= matRotZ(worldTransforms_[i]);
		worldTransforms_[i].matWorld_ *= matRotX(worldTransforms_[i]);
		worldTransforms_[i].matWorld_ *= matRotY(worldTransforms_[i]);
		worldTransforms_[i].matWorld_ *= matTrans(worldTransforms_[i]);

		if (i >= 1) {
			worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
		}
		worldTransforms_[i].TransferMatrix();
	}


	debugText_->SetPos(50, 90);
	debugText_->Printf("transration(%f,%f,%f)", worldTransforms_[kRoot].translation_.x, worldTransforms_[kRoot].translation_.y, worldTransforms_[kRoot].translation_.z);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画

	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (int i = 2; i < 9; i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画前処理
	for (int i = 0; i < 11; i++)
	{
		PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(0, i, 0), Vector3(10, i, 0), Vector4(255, 255, 255, 255));
		PrimitiveDrawer::GetInstance()->DrawLine3d(Vector3(i, 0, 0), Vector3(i, 10, 0), Vector4(255, 255, 255, 255));
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
