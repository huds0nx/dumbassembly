#pragma once

namespace dotnetpe
{

    class ILInstr
    {
    public:
        static int          GetLength               ( byte* pInstr );

        static bool         IsConstIntLoad          ( byte* pInstr );
        static int          GetConstIntLoadValue    ( byte* pInstr );

        static bool         IsBranch                ( byte* pInstr );
        static bool         IsConditionalBranch     ( byte* pInstr );
        static bool         IsUnconditionalBranch   ( byte* pInstr );
        static byte         GetBranchType           ( byte* pInstr );
        static byte*        GetBranchTarget         ( byte* pInstr );

        static bool         IsLeave                 ( byte* pInstr );
        static byte*        GetLeaveTarget          ( byte* pInstr );

        static bool         IsCall                  ( byte* pInstr );
        static bool         IsDirectCall            ( byte* pInstr );
        static bool         IsIndirectCall          ( byte* pInstr );

        static bool         IsSwitch                ( byte* pInstr );
        static dword        NumSwitchTargets        ( byte* pInstr );
        static byte*        GetSwitchTarget         ( byte* pInstr, dword dwTargetIndex );

        static void         WriteBranch             ( byte ucType, byte* pAt, byte* pTarget );
        static void         WriteBr                 ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BR_S, ILOPCODE_BR, pAt, pTarget ); }
        static void         WriteBrTrue             ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BRTRUE_S, ILOPCODE_BRTRUE, pAt, pTarget ); }
        static void         WriteBrFalse            ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BRFALSE_S, ILOPCODE_BRFALSE, pAt, pTarget ); }
        static void         WriteBeq                ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BEQ_S, ILOPCODE_BEQ, pAt, pTarget ); }
        static void         WriteBlt                ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BLT_S, ILOPCODE_BLT, pAt, pTarget ); }
        static void         WriteBle                ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BLE_S, ILOPCODE_BLE, pAt, pTarget ); }
        static void         WriteBge                ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BGE_S, ILOPCODE_BGE, pAt, pTarget ); }
        static void         WriteBgt                ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BGT_S, ILOPCODE_BGT, pAt, pTarget ); }
        static void         WriteBneUn              ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BNE_UN_S, ILOPCODE_BNE_UN, pAt, pTarget ); }
        static void         WriteBltUn              ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BLT_UN_S, ILOPCODE_BLT_UN, pAt, pTarget ); }
        static void         WriteBleUn              ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BLE_UN_S, ILOPCODE_BLE_UN, pAt, pTarget ); }
        static void         WriteBgeUn              ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BGE_UN_S, ILOPCODE_BGE_UN, pAt, pTarget ); }
        static void         WriteBgtUn              ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_BGT_UN_S, ILOPCODE_BGT_UN, pAt, pTarget ); }

        static void         WriteLeave              ( byte* pAt, byte* pTarget )    { WriteBranch ( ILOPCODE_LEAVE_S, ILOPCODE_LEAVE, pAt, pTarget ); }

        static void         WriteSwitch             ( byte* pAt, dword dwNumTargets, byte** ppTargets );
        static void         WriteSwitch             ( byte* pAt, dword dwNumTargets, fastdelegate::FastDelegate1 < dword, byte* > dgGetTarget );

        static void         WriteBranch             ( byte ucType, Stream* pStream, int iOffset );
        static void         WriteBr                 ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BR_S, ILOPCODE_BR, pStream, iOffset ); }
        static void         WriteBrTrue             ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BRTRUE_S, ILOPCODE_BRTRUE, pStream, iOffset ); }
        static void         WriteBrFalse            ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BRFALSE_S, ILOPCODE_BRFALSE, pStream, iOffset ); }
        static void         WriteBeq                ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BEQ_S, ILOPCODE_BEQ, pStream, iOffset ); }
        static void         WriteBlt                ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BLT_S, ILOPCODE_BLT, pStream, iOffset ); }
        static void         WriteBle                ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BLE_S, ILOPCODE_BLE, pStream, iOffset ); }
        static void         WriteBge                ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BGE_S, ILOPCODE_BGE, pStream, iOffset ); }
        static void         WriteBgt                ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BGT_S, ILOPCODE_BGT, pStream, iOffset ); }
        static void         WriteBneUn              ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BNE_UN_S, ILOPCODE_BNE_UN, pStream, iOffset ); }
        static void         WriteBltUn              ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BLT_UN_S, ILOPCODE_BLT_UN, pStream, iOffset ); }
        static void         WriteBleUn              ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BLE_UN_S, ILOPCODE_BLE_UN, pStream, iOffset ); }
        static void         WriteBgeUn              ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BGE_UN_S, ILOPCODE_BGE_UN, pStream, iOffset ); }
        static void         WriteBgtUn              ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_BGT_UN_S, ILOPCODE_BGT_UN, pStream, iOffset ); }

        static void         WriteLeave              ( Stream* pStream, int iOffset )    { WriteBranch ( ILOPCODE_LEAVE_S, ILOPCODE_LEAVE, pStream, iOffset ); }

        static void         WriteSwitch             ( Stream* pStream, dword dwNumTargets, int* pOffsets );
        static void         WriteSwitch             ( Stream* pStream, dword dwNumTargets, fastdelegate::FastDelegate1 < dword, int > dgGetOffset );

        static void         RetargetBranches        ( byte* pBranchesBegin, int iBranchesLength, byte* pTargetBegin, int iTargetLength, byte* pNewTarget );
        static void         MoveCode                ( byte* pTo, byte* pFrom, int iSize );

    private:
        static void         WriteBranch             ( byte ucShortOpcode, byte ucLongOpcode, byte* pAt, byte* pTarget );
        static void         WriteBranch             ( byte ucShortOpcode, byte ucLongOpcode, Stream* pStream, int iOffset );
    };

}
