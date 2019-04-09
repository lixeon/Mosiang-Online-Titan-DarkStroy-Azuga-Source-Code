namespace web.admin
{
    using System;
    using System.Data;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpcharactercreatename : Page
    {
        protected Button btnsearch;
        protected DataGrid DataGrid2;
        private static DataSet ds;
        protected TextBox tbcname;

        private void btnsearch_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbcname.Text.ToString().Trim());
            str = new system().ConvertToBig5(str, 0x3a8);
            string mySql = "select * from [MHGAME].[dbo].[TB_CharacterCreateInfo] where CHARACTER_NAME like '%" + str + "%'";
            ds = new DataProviders().ExecuteSqlDs(mySql, "DataGrid1");
            for (int i = 0; i < ds.Tables[0].Rows.Count; i++)
            {
                ds.Tables[0].Rows[i]["character_name"] = new system().ConvertToBig5(ds.Tables[0].Rows[i]["character_name"].ToString(), 950);
            }
            this.DataGrid2.DataSource = ds;
            this.DataGrid2.DataBind();
        }

        private void InitializeComponent()
        {
            this.btnsearch.Click += new EventHandler(this.btnsearch_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isadmin();
        }
    }
}
