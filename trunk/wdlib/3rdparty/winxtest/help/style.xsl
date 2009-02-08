<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">
	<html>
		<body>
			<xsl:for-each select="TestResult/TestCase">
				<font face="Calibri">
				<table width="100%" border="1" cellpadding="0" cellspacing="0">
					<tr>
						<td height="25" bgcolor="525D76">
							<div align="center">
								<font color="#FFFFFF">
									<strong>案例</strong>
								</font>
							</div>
						</td>
						<td width="90%" height="25" bgcolor="525D76">
							<div>
								<font color="#FFFFFF">
									<xsl:value-of select="Name"/>
								</font>
							</div>
						</td>						
					</tr>
					<tr>
						<td height="25" bgcolor="525D76">
							<div align="center">
								<font color="#FFFFFF">
									<strong>位置</strong>
								</font>
							</div>
						</td>
						<td width="90%" height="25">
							<div>
								<xsl:value-of select="Pos"/>
							</div>
						</td>						
					</tr>
					<tr>
						<td height="50" bgcolor="525D76">
							<div align="center">
								<font color="#FFFFFF">
									<strong>信息</strong>
								</font>
							</div>
						</td>
						<td width="90%" height="50">
							<div>
								<xsl:value-of select="Info"/>
							</div>
						</td>						
					</tr>
				</table>
				</font>
				<br/>
			</xsl:for-each>
		</body>
	</html>
</xsl:template>
</xsl:stylesheet>