#include "phymod.h"

Modifier* FindPhysiqueModifier (INode* nodePtr)
{
	// Get object from node. Abort if no object.
	Object* ObjectPtr = nodePtr->GetObjectRef();
			

	if (!ObjectPtr) return NULL;

	// Is derived object ?
	while (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID && ObjectPtr)
	{
		// Yes -> Cast.
		IDerivedObject *DerivedObjectPtr = (IDerivedObject *)(ObjectPtr);
						
		// Iterate over all entries of the modifier stack.
		int ModStackIndex = 0;
		while (ModStackIndex < DerivedObjectPtr->NumModifiers())
		{
			// Get current modifier.
			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);

			// Is this Physique ?
			if (ModifierPtr->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
			{
				// Yes -> Exit.
				return ModifierPtr;
			}

			// Next modifier stack entry.
			ModStackIndex++;
		}
		ObjectPtr = DerivedObjectPtr->GetObjRef();
	}

	// Not found.
	return NULL;
}


BOOL IsPhysiqueObject(INode* pNode)
{
	BOOL	bResult = FALSE;
	Modifier *phyMod = FindPhysiqueModifier(pNode);
	ObjectState os = pNode->EvalWorldState(0);
	
	if (!phyMod) 
		return FALSE;
	
	// create a Physique Export Interface for the given Physique Modifier
	IPhysiqueExport *phyExport = (IPhysiqueExport *)phyMod->GetInterface(I_PHYINTERFACE);
	
	if (phyExport)
	{
		// create a ModContext Export Interface for the specific node of the Physique Modifier
		IPhyContextExport *mcExport 
			= (IPhyContextExport *)phyExport->GetContextInterface(pNode);
		
		if (mcExport)
		{
			phyExport->ReleaseContextInterface(mcExport);
			bResult = TRUE;
		}
		phyMod->ReleaseInterface(I_PHYINTERFACE, phyExport);
	}
	return bResult;
}