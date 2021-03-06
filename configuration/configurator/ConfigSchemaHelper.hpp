/*##############################################################################

    HPCC SYSTEMS software Copyright (C) 2015 HPCC Systems®.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
############################################################################## */

#ifndef _CONFIG_SCHEMA_HELPER_HPP_
#define _CONFIG_SCHEMA_HELPER_HPP_

#include "jiface.hpp"
#include "jptree.hpp"
#include "jutil.hpp"
#include "jarray.hpp"
#include "jhash.hpp"
#include "SchemaAttributes.hpp"
#include "SchemaAttributeGroup.hpp"
#include "SchemaElement.hpp"
#include "SchemaSchema.hpp"
#include "SchemaExtension.hpp"
#include "BuildSet.hpp"
#include "ConfiguratorAPI.hpp"
#include "SchemaMapManager.hpp"

namespace CONFIGURATOR
{
static const char* CONFIGURATOR_HEADER = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Environment>\n\t<Software>";

class CSchemaMapManager;
class CSimpleType;

class CConfigSchemaHelper : public ::CInterface
{
public:

    IMPLEMENT_IINTERFACE

    static CConfigSchemaHelper* getInstance(const char* pDefaultDirOverride =  nullptr);
    static CConfigSchemaHelper* getInstance(const char* pBuildSetFileName, const char *pBaseDirectory, const char *pDefaultDirOverride = nullptr);
    static CConfigSchemaHelper* getNewInstance(const char* pDefaultDirOverride =  nullptr);

    virtual ~CConfigSchemaHelper();

    bool populateSchema();
    void printConfigSchema(StringBuffer &str) const;

    CSchemaMapManager* getSchemaMapManager()
    {
        return m_pSchemaMapManager;
    }

    void addExtensionToBeProcessed(CExtension *pExtension);
    void processExtensionArr();

    void addAttributeGroupToBeProcessed(CAttributeGroup *pAttributeGroup);
    void processAttributeGroupArr();

    void addNodeForBaseProcessing(CXSDNodeWithBase *pNode);
    void processNodeWithBaseArr();

    void addNodeForTypeProcessing(CXSDNodeWithType *pNode);
    void processNodeWithTypeArr(CXSDNodeBase *pParentNode = nullptr);

    void addElementForRefProcessing(CElement *pElement);
    void processElementArr();

    void addKeyRefForReverseAssociation(const CKeyRef *pKeyRef) const;
    void processKeyRefReverseAssociation() const;

    void addKeyForReverseAssociation(const CKey *pKeyRef) const;
    void processKeyReverseAssociation() const;

    bool getXMLFromSchema(StringBuffer& strXML, const char* pXSD); //test purposes
    void populateEnvXPath();
    void loadEnvFromConfig(const char *pEnvFile);
    void printDocumentation(const char* comp, char **pOutput) const;
    void printJSON(const char* comp, char **pOutput, int nIdx = -1, bool bCleanUp = false) const;
    void printJSONByKey(const char* key, char **pOutput, bool bCleanUp = false) const;
    void printNavigatorJSON(char **pOutput, bool bCleanUp = false) const;
    void printDump(const char* comp) const;
    void dumpStdOut() const;

    const char* getBasePath() const
    {
        return m_pBasePath;
    }

    void setBasePath(const char *pBasePath);
    void setEnvTreeProp(const char *pXPath, const char* pValue);
    const char* getTableValue(const char* pXPath, int nRow = 1) const;

    int getEnvironmentXPathSize() const
    {
        return m_strArrayEnvXPaths.length();
    }
    const char* getEnvironmentXPaths(int idx) const
    {
        assert(idx >= 0);
        assert(m_strArrayEnvXPaths.length() > idx);
        return m_strArrayEnvXPaths.item(idx);
    }

    const char* getAttributeXSDXPathFromEnvXPath(const char* pEnvXPath) const;
    const char* getElementArrayXSDXPathFromEnvXPath(const char* pXSDXPath) const;
    int getElementArraySize(const char *pXPath) const;
    void appendAttributeXPath(const char *pXPath);
    void appendElementXPath(const char *pXPath);

    static int stripXPathIndex(StringBuffer &strXPath);
    static bool isXPathTailAttribute(const StringBuffer &strXPath);

    ::IPropertyTree* getEnvPropertyTree()
    {
        return m_pEnvPropertyTree;
    }
    const ::IPropertyTree* getConstEnvPropertyTree() const
    {
        return m_pEnvPropertyTree;
    }
    int getNumberOfTables() const
    {
        return m_nTables;
    }
    void incTables()
    {
        m_nTables++;
    }
    void resetTables() const
    {
        m_nTables = 0;
    }
    bool saveConfigurationFile() const;
    bool saveConfigurationFileAs(const char *pFilePath = nullptr);

    const char* getEnvFilePath() const
    {
        return m_strEnvFilePath.str();
    }

    int getInstancesOfComponentType(const char *pCompType) const;

    const char* getInstanceNameOfComponentType(const char *pCompType, int idx);

protected:

    CConfigSchemaHelper(const char* pBuildSetFile = DEFAULT_BUILD_SET_XML_FILE, const char* pBuildSetDir = DEFAULT_BUILD_SET_DIRECTORY, const char* pDefaultDirOverride = nullptr);

    CSchemaMapManager *m_pSchemaMapManager;
    ::CIArrayOf<CExtension> m_extensionArr;
    ::CIArrayOf<CAttributeGroup> m_attributeGroupArr;
    ::CIArrayOf<CXSDNodeWithType> m_nodeWithTypeArr;
    ::CIArrayOf<CXSDNodeWithBase> m_nodeWithBaseArr;
    ::CIArrayOf<CElement> m_ElementArr;
    ::CIArrayOf<CKeyRef> m_KeyRefArr;
    ::StringArray m_strArrayEnvXPaths;
    ::StringArray m_strArrayEnvXMLComponentInstances;

    void setEnvPropertyTree(::IPropertyTree *pEnvTree)
    {
        m_pEnvPropertyTree =  pEnvTree;
    }
    void setEnvFilePath(const char* pEnvFilePath)
    {
        assert(pEnvFilePath != nullptr);
        m_strEnvFilePath.set(pEnvFilePath);
    }

    static void clearLF(StringBuffer& strToClear);
    static size_t getXPathIndexLength(const char *pXPath);

private:

    static CConfigSchemaHelper* s_pCConfigSchemaHelper;
    mutable int m_nTables;
    char *m_pBasePath;

    ::StringBuffer m_strEnvFilePath;
    ::IPropertyTree *m_pEnvPropertyTree;
};
}
#endif // _CONFIG_SCHEMA_HELPER_HPP_
