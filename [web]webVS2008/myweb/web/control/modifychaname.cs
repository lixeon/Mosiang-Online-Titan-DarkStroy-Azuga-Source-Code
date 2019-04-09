namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class modifychaname : UserControl
    {
        protected Button btnedit;
        protected DropDownList ddchalist;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator Requiredfieldvalidator6;
        protected TextBox tbnewname;

        private void btnedit_Click(object sender, EventArgs e)
        {
            int useridx = int.Parse(base.Session["useridx"].ToString());
            int chaidx = int.Parse(this.ddchalist.SelectedValue.ToString());
            string str = new system().ChkSql(new system().ConvertToBig5(this.tbnewname.Text.ToString().Trim(), 0x3a8));
            int num3 = new system().getStrLen(str);
            if ((num3 < 4) | (num3 > 12))
            {
                base.Response.Write("<script language=javascript>alert('角色名長度不符合要求，名稱必需4至12的長度')</script>");
            }
            else
            {
                string str2 = new WebLogic().modifychaname(base.Session["userid"].ToString(), useridx, chaidx, str, int.Parse(base.Application["game.modifychanamecount"].ToString()), int.Parse(base.Application["game.modifychanamemoney"].ToString()));
                base.Response.Write("<script language=javascript>alert('" + str2 + "')</script>");
            }
        }

        private void InitializeComponent()
        {
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            if (base.Session["userid"] == null)
            {
                base.Response.Redirect("login.aspx?returnurl=" + base.Request.ServerVariables["SCRIPT_NAME"]);
            }
            else if (!this.Page.IsPostBack)
            {
                this.ddchalist.DataSource = new DataProviders().ExecuteSqlDs("select character_idx,character_name from mhgame..tb_character  where substring(character_name,1,1)!='@' and user_idx=" + base.Session["useridx"].ToString(), "ddchalist");
                this.ddchalist.DataTextField = "character_name";
                this.ddchalist.DataValueField = "character_idx";
                this.ddchalist.DataBind();
                for (int i = 0; i < this.ddchalist.Items.Count; i++)
                {
                    this.ddchalist.Items[i].Text = new system().ConvertToBig5(this.ddchalist.Items[i].Text.ToString(), 950);
                }
            }
        }
    }
}
