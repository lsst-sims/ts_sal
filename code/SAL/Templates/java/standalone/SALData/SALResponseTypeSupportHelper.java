package SALData;

import org.opensplice.dds.dcps.Utilities;

public final class SALResponseTypeSupportHelper
{

    public static SALData.SALResponseTypeSupport narrow(java.lang.Object obj)
    {
        if (obj == null) {
            return null;
        } else if (obj instanceof SALData.SALResponseTypeSupport) {
            return (SALData.SALResponseTypeSupport)obj;
        } else {
            throw Utilities.createException(Utilities.EXCEPTION_TYPE_BAD_PARAM, null);
        }
    }

    public static SALData.SALResponseTypeSupport unchecked_narrow(java.lang.Object obj)
    {
        if (obj == null) {
            return null;
        } else if (obj instanceof SALData.SALResponseTypeSupport) {
            return (SALData.SALResponseTypeSupport)obj;
        } else {
            throw Utilities.createException(Utilities.EXCEPTION_TYPE_BAD_PARAM, null);
        }
    }

}
