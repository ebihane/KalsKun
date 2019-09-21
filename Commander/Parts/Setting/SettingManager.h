#pragma once

#include "Include/Common.h"

class SettingManager
{
public :

    /* �C���X�^���X�擾 */
    static SettingManager* const GetInstance();

    /* �f�t�H���g�f�[�^���� */
    void CreateDefaultData();

    /* �t�@�C������ */
    bool IsFileExist();     /* �t�@�C���L���m�F */
    ResultEnum Save();      /* �ۑ� */
    ResultEnum Load();      /* �ǂݍ��� */
    int GetLastErrorNo();   /* �G���[�ԍ��擾 */

    /*--------------------*/
    /* �ݒ�l�̎擾�E�ύX */
    /*--------------------*/
    /* ���{�b�g�T�C�Y */
    SizeStr GetRobotSize();                     /* �擾 */
    void SetRobotSize(SizeStr* const size);     /* �ݒ� */

    /* ���̃T�C�Y */
    SizeStr GetFarmSize();                      /* �擾 */
    void SetFarmSize(SizeStr* const size);      /* �ݒ� */

    /* �}�b�v�̌� */
    RectStr GetMapCount();                      /* �擾 */

    /* �d�g�̓`���W�� */
    float GetWavePowerCoeff();                  /* �擾 */
    void SetWavePowerCoeff(const float coeff);  /* �ݒ� */

    /* �A�N�Z�X�|�C���g */
    long GetApCount();                          /* �� �擾 */
    void GetApAddress(RectStr* data);           /* ���W �擾 */
    void SetApInfo(const long count, RectStr* address); /* ���{���W �ݒ� */

protected :


private :

    static SettingManager* m_This;  /* ���N���X�C���X�^���X */

    int         m_LastErrorNo;      /* �Ō�ɔ��������V�X�e���G���[ */

    SizeStr     m_RobotSize;        /* ���{�b�g�T�C�Y */
    SizeStr     m_FarmSize;         /* ���̃T�C�Y */
    RectStr     m_MapCount;         /* �}�b�v�̌� */
    float       m_WavePowerCoeff;   /* �d�g�̓`���W�� */
    long        m_ApCount;          /* �A�N�Z�X�|�C���g�̌� */
    RectStr*    m_ApAddress;        /* �A�N�Z�X�|�C���g�̍��W */

    SettingManager();
    virtual ~SettingManager();

    long calcurateSettingSize();    /* �ݒ�̍��v�T�C�Y�擾 */
    void parseString(char* const src, char* const dest, const char delimiter, const long maxLen);

};