//#include <cryptopp/sha3.h>

#include "Schnorr.h"

#ifndef _SCHNORR_CPP
#define _SCHNORR_CPP

CCurve::CCurve()
{
	secretKeySet = false;
	publicKeySet = false;

	// Load in curve secp256r1
	Integer p, a, b, Gx, Gy;
	p = Integer("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F");
	a = Integer("0");
	b = Integer("0x7");
	q = Integer("0xFFFFFFFF00000000FFFFFFFFFFFFFFFFBCE6FAADA7179E84F3B9CAC2FC632551"); //产生随机数用
	Gx = Integer("0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");
	Gy = Integer("0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");

	// Store the curve and the generator
	ec = ECP(p, a, b);
	G = ECPPoint(Gx, Gy);
}

CCurve::~CCurve()
{
	secretKeySet = false;
	publicKeySet = false;
}

//Integer CCurve::HashPointMessage(const ECPPoint& R,
//	const byte* message, int mlen)
//{
//	const int digestsize = 256/8;
//	SHA3 sha(digestsize);

//	int len = ec.EncodedPointSize();
//	byte *buffer = new byte[len];
//	ec.EncodePoint(buffer, R, false);
//	sha.Update(buffer, len);
//	delete[] buffer;

//	sha.Update(message, mlen);

//	byte digest[digestsize];
//	sha.Final(digest);

//	Integer ans;
//	ans.Decode(digest, digestsize);
//	return ans;
//}

bool CCurve::GenerateSecretKey()
{
	secretKey = Integer(rng, 256) % q;
	secretKeySet = true;
	return true;
}

bool CCurve::GeneratePublicKey()
{
	if (!secretKeySet)
		return false;
	Q = ec.ScalarMultiply(G, secretKey);
	publicKeySet = true;
	return true;
}

bool CCurve::GenerateKeys()
{
	if (!GenerateSecretKey())
		return false;
	if (!GeneratePublicKey())
		return false;
	return true;
}

//void CCurve::Sign(Integer& sigE, Integer& sigS, const byte* message, int mlen)
//{
//	Integer k;
//	ECPPoint R;
//	k = Integer(rng, 256) % q;
//	R = ec.ScalarMultiply(G, k);
//	sigE = HashPointMessage(R, message, mlen) % q;
//	sigS = (k - secretKey*sigE) % q;
//}

//bool CCurve::Verify(const Integer& sigE, const Integer& sigS,
//	const byte* message, int mlen)
//{
//	ECPPoint R;
//	R = ec.CascadeScalarMultiply(G, sigS, Q, sigE);
//	Integer sigEd = HashPointMessage(R, message, mlen) % q;
//	return (sigE == sigEd);
//}

Integer CCurve::GetPublicKeyX()
{
	//if (!publicKeySet)
	//	throw new key_error("Public key not set");

	return Q.x;
}

Integer CCurve::GetPublicKeyY()
{
	//if (!publicKeySet)
	//	throw new key_error("Public key not set");

	return Q.y;
}

Integer CCurve::GetSecretKey()
{
	//if (!secretKeySet)
	//	throw new key_error("Secret key not set");

	return secretKey;
}

bool CCurve::SetVchPublicKey(std::vector<unsigned char> vchPubKey)
{
	ECPPoint publicKey;

	if (!ec.DecodePoint (publicKey, &vchPubKey[0], vchPubKey.size()))
	return false;

	Q = publicKey;
	return true;
}

bool CCurve::GetVchPublicKey(std::vector<unsigned char>& vchPubKey)
{
	// set to true for compressed
	const bool fCompressed = true;
	vchPubKey.resize(ec.EncodedPointSize(fCompressed));
	ec.EncodePoint(&vchPubKey[0], Q, fCompressed);
	return true;
}

bool CCurve::GetSignatureFromVch(std::vector<unsigned char> vchSig, Integer& sigE, Integer& sigS)
{
	if (vchSig.size() != (SCHNORR_SIG_SIZE * 2))
	return false;

	// extract bytes
	std::vector<unsigned char> sigEVec(&vchSig[0], &vchSig[SCHNORR_SIG_SIZE]);
	std::vector<unsigned char> sigSVec(&vchSig[SCHNORR_SIG_SIZE], &vchSig[1 + SCHNORR_SIG_SIZE * 2]);

	// vectors -> Integers
	sigE.Decode(&sigEVec[0], SCHNORR_SIG_SIZE);
	sigS.Decode(&sigSVec[0], SCHNORR_SIG_SIZE);
	return true;
}

bool CCurve::GetVchFromSignature(std::vector<unsigned char>& vchSig, Integer sigE, Integer sigS)
{
	vchSig.resize(SCHNORR_SIG_SIZE * 2);

	if (sigE.MinEncodedSize() > SCHNORR_SIG_SIZE || sigS.MinEncodedSize() > SCHNORR_SIG_SIZE)
	return false;

	sigE.Encode(&vchSig[0], SCHNORR_SIG_SIZE);
	sigS.Encode(&vchSig[SCHNORR_SIG_SIZE], SCHNORR_SIG_SIZE);
	return true;
}

bool CCurve::SetVchSecretKey(std::vector<unsigned char> vchSecret)
{
	if (vchSecret.size() != SCHNORR_SECRET_KEY_SIZE)
	return false;

	secretKey.Decode(&vchSecret[0], SCHNORR_SECRET_KEY_SIZE);
	return true;
}

bool CCurve::GetVchSecretKey(std::vector<unsigned char>& vchSecret)
{
	if (!secretKeySet)
	return false;

	vchSecret.resize(SCHNORR_SECRET_KEY_SIZE);
	secretKey.Encode(&vchSecret[0], SCHNORR_SECRET_KEY_SIZE);
	return true;
}
#endif
